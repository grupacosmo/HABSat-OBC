//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "lcd.h"
#include <array>
#include "bitwise_operations.h"
#include "obc.h"

// TODO:
// SWAP HAL_Delay() FUNCTION FOR A DELAY FUNCTION THAT ACCEPTS MICROSECONDS AS AN ARGUMENT
// FIND MISSING DELAYS
// CHECK DATASHEET AND ARDUINO I2C LIQUID CRYSTAL LIB FOR ACCURATE DELAY USAGE

Lcd::Lcd(const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *const hi2cx, uint8_t slave_address)
: m_hi2cx(hi2cx),
  m_lines(lines),
  m_line_length(line_length),
  m_slave_address(slave_address),
  m_display_ctrl_config(std::byte{Command::DISPLAY_CRTL} | std::byte{DisplayCtrlFlag::DISPLAY_STATE}),
  m_entry_mode_config(std::byte{Command::ENTRY_MODE} | std::byte{EntryModeFlag::SHIFT_DIRECTION})
{}


void Lcd::init() const
{
    std::byte display_lines_flag{NO_FLAG};
    if(m_lines > 1)
        display_lines_flag = std::byte{FunctionSetFlag::DISPLAY_LINES};

    auto wait_and_send_cmd = [this](uint32_t time, std::byte b)
    {
        HAL_Delay(time);
        send_cmd(b);
    };
    wait_and_send_cmd(20, std::byte{Command::FUNCTION_SET});
    wait_and_send_cmd(5, std::byte{Command::FUNCTION_SET});
    wait_and_send_cmd(1, std::byte{Command::FUNCTION_SET});
    wait_and_send_cmd(1, std::byte{Command::FUNCTION_SET} | display_lines_flag);
    wait_and_send_cmd(1, m_display_ctrl_config);
    wait_and_send_cmd(1, std::byte{Command::CLEAR_DISPLAY});
    wait_and_send_cmd(1, m_entry_mode_config);

    display_task.add_to_scheduler();
}

void Lcd::print_line(const u_int16_t line_index, const std::string& str) const
{
    set_cursor_pos(line_index, 0);

    size_t i;
    for(i = 0; i < m_line_length && i < str.size(); i++)
    {
        send_data(std::byte{str[i]});
    }

    for(; i < m_line_length; i++)
    {
        send_data(std::byte{' '});
    }

    set_cursor_pos(line_index, str.size());
}

void Lcd::print_char(const char character) const
{
    send_data(std::byte{character});
}

void Lcd::set_cursor_pos(uint16_t line, const uint16_t column) const
{
    const std::array<LineOffset, 4> line_offsets {
        LineOffset::LINE_0,
        LineOffset::LINE_1,
        LineOffset::LINE_2,
        LineOffset::LINE_3,
    };

    if(line >= m_lines)
        line = m_lines - 1;

    const uint8_t address = column + static_cast<u_int8_t>(line_offsets[line]);

    send_cmd(std::byte{Command::DDRAM_ADDRESS} | std::byte{address});
}

void Lcd::clear() const
{
    send_cmd(Command::CLEAR_DISPLAY);
    // TODO DELAY
    HAL_Delay(1);
}

void Lcd::display_on()
{
    set_flag(m_display_ctrl_config, std::byte{DisplayCtrlFlag::DISPLAY_STATE});
}

void Lcd::display_off()
{
    unset_flag(m_display_ctrl_config, std::byte{DisplayCtrlFlag::DISPLAY_STATE});
}

void Lcd::cursor_on()
{
    set_flag(m_display_ctrl_config, std::byte{DisplayCtrlFlag::CURSOR_STATE});
}

void Lcd::cursor_off()
{
    unset_flag(m_display_ctrl_config, std::byte{DisplayCtrlFlag::CURSOR_STATE});
}

void Lcd::cursor_blinking()
{
    set_flag(m_display_ctrl_config, std::byte{DisplayCtrlFlag::CURSOR_BLINK});
}

void Lcd::cursor_not_blinking()
{
    unset_flag(m_display_ctrl_config, std::byte{DisplayCtrlFlag::CURSOR_BLINK});
}

void Lcd::shift_mode_display()
{
    set_flag(m_entry_mode_config, std::byte{EntryModeFlag::SHIFT_TYPE});
}

void Lcd::shift_mode_cursor()
{
    unset_flag(m_entry_mode_config, std::byte{EntryModeFlag::SHIFT_TYPE});
}

void Lcd::shift_direction_left()
{
    set_flag(m_entry_mode_config, std::byte{EntryModeFlag::SHIFT_DIRECTION});
}

void Lcd::shift_direction_right()
{
    unset_flag(m_entry_mode_config, std::byte{EntryModeFlag::SHIFT_DIRECTION});
}

void Lcd::set_flag(std::byte& config, const std::byte& flag)
{
    config |= flag;
    send_cmd(config);
}

void Lcd::unset_flag(std::byte& config, const std::byte& flag)
{
    config &= ~flag;
    send_cmd(config);
}

void Lcd::send_cmd(const std::byte& byte) const
{
    send(byte, std::byte{DataSendingFlag::BACKLIGHT});
}

void Lcd::send_cmd(const Command &cmd) const
{
    send_cmd(std::byte{cmd});
}

void Lcd::send_data(const std::byte &data) const
{
    send(data, std::byte{DataSendingFlag::REGISTER} | std::byte{DataSendingFlag::BACKLIGHT});
}

void Lcd::send(const std::byte& byte, const std::byte& flags) const
{
    // since I2C requires 4 bit interface, bytes of data need to be split into nibbles
    transmit_nibble(bitwise::nibble_high(byte) | flags);
    transmit_nibble((bitwise::nibble_low(byte) << 4) | flags);
}

void Lcd::transmit_nibble(const std::byte &nibble) const
{
    // the data needs to be transmitted twice
    // first time with PULSE flag, second time without it
    i2c_transmit(nibble | std::byte{DataSendingFlag::PULSE}, 1);
    // TODO DELAY > 450 ns

    i2c_transmit(nibble & ~std::byte{DataSendingFlag::PULSE}, 1);
    // TODO DELAY > 37 us
}

void Lcd::i2c_transmit(std::byte data, const uint16_t data_length) const
{
    HAL_I2C_Master_Transmit(m_hi2cx, m_slave_address, reinterpret_cast<uint8_t *>(&data), data_length, 100);
}

void Lcd::prepareHeaderData(std::array<char, 20> &lineBuffer)
{
    constexpr std::array options {"display", "test"};
    static std::size_t index;

    std::sprintf(lineBuffer.data(), "%s", options[index]);
    index = (index + 1) % options.size();
}

void Lcd::prepareTimeData(std::array<char, 20> &lineBuffer)
{
    static const auto &rtcBuffers = obc.peripherals.rtc.getDateTime();
    std::sprintf(lineBuffer.data(), "      %'.02d:%'.02d:%'.02d", rtcBuffers.hour, rtcBuffers.minute, rtcBuffers.second);
}

void Lcd::prepareDateData(std::array<char, 20> &lineBuffer)
{
    static const auto &rtcBuffers = obc.peripherals.rtc.getDateTime();
    // TODO: the first string is a weird workaround for program crashing when calling day_names[rtcBuffers.weekday_name - 1]
    static const std::array day_names = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    std::sprintf(lineBuffer.data(), "    %s %'.02d/%'.02d/%'.02d", day_names[rtcBuffers.weekday_name],
                 rtcBuffers.day, rtcBuffers.month, rtcBuffers.year);
}

void Lcd::prepareSensorData(std::array<char, 20> &lineBuffer)
{
    static const auto &sensorBuffers = obc.peripherals.sensor.getBuffers();
    static const std::array bufferArray{&sensorBuffers.temperature, &sensorBuffers.pressure, &sensorBuffers.humidity};

    static constexpr std::array options{"Temp: %.2lf C", "Press: %.2lf hPa", "Hum: %.2lf %%RH"};
    static std::size_t index = 0;

    std::sprintf(lineBuffer.data(), options[index], *bufferArray[index]);
    index = (index + 1) % options.size();
}

void Lcd::display_task_function(void *args)
{
    (void)args;
    const auto &lcd = obc.peripherals.lcd;
    std::array<std::array<char, 20>, 4> lineBuffers;

    while (true)
    {
        prepareHeaderData(lineBuffers[0]);
        prepareTimeData(lineBuffers[1]);
        prepareDateData(lineBuffers[2]);
        prepareSensorData(lineBuffers[3]);

        for(std::size_t i = 0; i < 4; ++i)
            lcd.print_line(i, lineBuffers[i].data());

        os::Task::delay(980);
    }
}