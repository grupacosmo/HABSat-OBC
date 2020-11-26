//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "lcd.h"
#include "bitwise_operations.h"
#include "obc.h"
#include <array>

namespace hw
{

// TODO:
// SWAP HAL_Delay() FUNCTION FOR A DELAY FUNCTION THAT ACCEPTS MICROSECONDS AS AN ARGUMENT
// FIND MISSING DELAYS
// CHECK DATASHEET AND ARDUINO I2C LIQUID CRYSTAL LIB FOR ACCURATE DELAY USAGE

Lcd::Lcd(uint16_t lines, uint16_t line_length, const I2CBus* i2c, uint8_t slave_address)
: i2c_(i2c),
      lines_(lines),
      lineLength_(line_length),
      slaveAddress_(slave_address),
      displayCtrlConfig_(Command::DISPLAY_CRTL | DisplayCtrlFlag::DISPLAY_STATE),
      entryModeConfig_(Command::ENTRY_MODE | EntryModeFlag::SHIFT_DIRECTION)
{}


void Lcd::init() const
{
    uint8_t display_lines_flag = NO_FLAG;
    if(lines_ > 1)
        display_lines_flag = FunctionSetFlag::DISPLAY_LINES;

    auto wait_and_send_cmd = [this](uint32_t time, uint8_t b)
    {
        HAL_Delay(time);
        send_cmd(b);
    };
    wait_and_send_cmd(20, Command::FUNCTION_SET);
    wait_and_send_cmd(5, Command::FUNCTION_SET);
    wait_and_send_cmd(1, Command::FUNCTION_SET);
    wait_and_send_cmd(1, Command::FUNCTION_SET | display_lines_flag);
    wait_and_send_cmd(1, displayCtrlConfig_);
    wait_and_send_cmd(1, Command::CLEAR_DISPLAY);
    wait_and_send_cmd(1, entryModeConfig_);

    displayTask_.addToScheduler();
}

void Lcd::print_line(const u_int16_t line_index, const std::string& str) const
{
    set_cursor_pos(line_index, 0);

    size_t i;
    for(i = 0; i < lineLength_ && i < str.size(); i++)
    {
        send_data(str[i]);
    }

    for(; i < lineLength_; i++)
    {
        send_data(' ');
    }

    set_cursor_pos(line_index, str.size());
}

void Lcd::print_char(const char character) const
{
    send_data(character);
}

void Lcd::set_cursor_pos(uint16_t line, const uint16_t column) const
{
    const std::array<LineOffset, 4> line_offsets {
        LineOffset::LINE_0,
        LineOffset::LINE_1,
        LineOffset::LINE_2,
        LineOffset::LINE_3,
    };

    if(line >= lines_)
        line = lines_ - 1;

    const uint8_t address = column + line_offsets[line];

    send_cmd(Command::DDRAM_ADDRESS | address);
}

void Lcd::clear() const
{
    send_cmd(Command::CLEAR_DISPLAY);
    // TODO DELAY
    HAL_Delay(1);
}

void Lcd::display_on()
{
    set_flag(displayCtrlConfig_, DisplayCtrlFlag::DISPLAY_STATE);
}

void Lcd::display_off()
{
    unset_flag(displayCtrlConfig_, DisplayCtrlFlag::DISPLAY_STATE);
}

void Lcd::cursor_on()
{
    set_flag(displayCtrlConfig_, DisplayCtrlFlag::CURSOR_STATE);
}

void Lcd::cursor_off()
{
    unset_flag(displayCtrlConfig_, DisplayCtrlFlag::CURSOR_STATE);
}

void Lcd::cursor_blinking()
{
    set_flag(displayCtrlConfig_, DisplayCtrlFlag::CURSOR_BLINK);
}

void Lcd::cursor_not_blinking()
{
    unset_flag(displayCtrlConfig_, DisplayCtrlFlag::CURSOR_BLINK);
}

void Lcd::shift_mode_display()
{
    set_flag(entryModeConfig_, EntryModeFlag::SHIFT_TYPE);
}

void Lcd::shift_mode_cursor()
{
    unset_flag(entryModeConfig_, EntryModeFlag::SHIFT_TYPE);
}

void Lcd::shift_direction_left()
{
    set_flag(entryModeConfig_, EntryModeFlag::SHIFT_DIRECTION);
}

void Lcd::shift_direction_right()
{
    unset_flag(entryModeConfig_, EntryModeFlag::SHIFT_DIRECTION);
}

void Lcd::set_flag(uint8_t& config, uint8_t flag)
{
    config |= flag;
    send_cmd(config);
}

void Lcd::unset_flag(uint8_t& config, uint8_t flag)
{
    config &= ~flag;
    send_cmd(config);
}

void Lcd::send_cmd(uint8_t byte) const
{
    send(byte, DataSendingFlag::BACKLIGHT);
}

void Lcd::send_data(uint8_t data) const
{
    send(data, DataSendingFlag::REGISTER | DataSendingFlag::BACKLIGHT);
}

void Lcd::send(uint8_t byte, uint8_t flags) const
{
    // since I2C requires 4 bit interface, bytes of data need to be split into nibbles
    transmit_nibble(bitwise::nibbleHigh(byte) | flags);
    transmit_nibble((bitwise::nibbleLow(byte) << 4) | flags);
}

void Lcd::transmit_nibble(uint8_t nibble) const
{
    // the data needs to be transmitted twice
    // first time with PULSE flag, second time without it
    uint8_t data = nibble | DataSendingFlag::PULSE;
    i2c_->transmit(slaveAddress_, &data, 1);
    // TODO DELAY > 450 ns

    data = nibble & ~DataSendingFlag::PULSE;
    i2c_->transmit(slaveAddress_, &data, 1);
    // TODO DELAY > 37 us
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
    static const auto &rtcBuffers = obc().hardware.rtc.getTimeAndDateBuffer();
    std::sprintf(lineBuffer.data(), "      %'.02d:%'.02d:%'.02d", rtcBuffers.hour, rtcBuffers.minute, rtcBuffers.second);
}

void Lcd::prepareDateData(std::array<char, 20> &lineBuffer)
{
    static const auto &rtcBuffers = obc().hardware.rtc.getTimeAndDateBuffer();
    // TODO: the first string is a weird workaround for program crashing when calling day_names[rtcBuffers.weekday_name - 1]
    static const std::array day_names = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    std::sprintf(lineBuffer.data(), "    %s %'.02d/%'.02d/%'.02d", day_names[rtcBuffers.weekday_name],
                 rtcBuffers.day, rtcBuffers.month, rtcBuffers.year);
}

void Lcd::prepareSensorData(std::array<char, 20> &lineBuffer)
{
    static const auto &sensorBuffers = obc().hardware.sensor.getBuffers();
    static const std::array bufferArray{&sensorBuffers.temperature, &sensorBuffers.pressure, &sensorBuffers.humidity};

    static constexpr std::array options{"Temp: %.2lf C", "Press: %.2lf hPa", "Hum: %.2lf %%RH"};
    static std::size_t index = 0;

    std::sprintf(lineBuffer.data(), options[index], *bufferArray[index]);
    index = (index + 1) % options.size();
}

void Lcd::display_task_function(void *args)
{
    (void)args;
    const auto &lcd = obc().hardware.lcd;
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

}