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






// public:

Lcd::Lcd(const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *const hi2cx, uint8_t slave_address)
: m_lines(lines),
  m_line_length(line_length),
  m_hi2cx(hi2cx),
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

    int i;
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

void Lcd::display_task_function(void *args)
{
    Lcd &lcd = obc.peripherals.lcd;
    auto &buffers = const_cast<Sensor::Buffers&>(obc.peripherals.sensor.getBuffers());

    auto lcd_formatted_print_line = [](short line, const char *text, float value) {
        std::array<char, 20> print_line_buffer;
        sprintf(&print_line_buffer[0], text, value);
        lcd.print_line(line, &print_line_buffer[0]);
    };

    constexpr std::array line_0_texts{"display", "test"};
    constexpr std::array line_3_texts{"Temp: %.2lf C", "Press: %.2lf hPa", "Hum: %.2lf %%RH"};
    const std::array<float*, 3> line_3_values{&buffers.temperature, &buffers.pressure, &buffers.humidity};

    std::size_t line_0_index = 0;
    std::size_t line_3_index = 0;
    while (true)
    {
        lcd.print_line(0, line_0_texts[line_0_index]);
        lcd_formatted_print_line(3, line_3_texts[line_3_index], *line_3_values[line_3_index]);

        line_0_index = (line_0_index + 1) % line_0_texts.size();
        line_3_index = (line_3_index + 1) % line_3_texts.size();

        os::Task::delay(1000);
    }
}

