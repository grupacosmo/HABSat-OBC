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

Lcd::Lcd(uint16_t lines, uint16_t lineLength, const I2CBus* i2c, uint8_t slaveAddress)
    : i2c_(i2c),
      lines_(lines),
      lineLength_(lineLength),
      slaveAddress_(slaveAddress),
      displayCtrlConfig_(Command::DisplayCtrl | DisplayCtrlFlag::DisplayState),
      entryModeConfig_(Command::EntryMode | EntryModeFlag::ShiftDirection)
{

}

void Lcd::init() const
{
    auto waitAndSendCmd = [this](uint32_t timeMs, uint8_t b) {
        HAL_Delay(timeMs);
        sendCmd(b);
    };

    waitAndSendCmd(20, Command::FunctionSet);
    waitAndSendCmd(5, Command::FunctionSet);
    waitAndSendCmd(1, Command::FunctionSet);
    if(lines_ > 1)
        waitAndSendCmd(1, Command::FunctionSet | FunctionSetFlag::DisplayLines);
    else
        waitAndSendCmd(1, Command::FunctionSet);
    waitAndSendCmd(1, displayCtrlConfig_);
    waitAndSendCmd(1, Command::ClearDisplay);
    waitAndSendCmd(1, entryModeConfig_);

    displayTask_.addToScheduler();
}

void Lcd::printLine(const u_int16_t lineIndex, const std::string& str) const
{
    setCursorPosition(lineIndex, 0);

    size_t i;
    for(i = 0; i < lineLength_ && i < str.size(); i++)
    {
        sendData(str[i]);
    }

    for(; i < lineLength_; i++)
    {
        sendData(' ');
    }

    setCursorPosition(lineIndex, str.size());
}

void Lcd::printChar(const char character) const
{
    sendData(character);
}

void Lcd::setCursorPosition(uint16_t line, const uint16_t column) const
{
    static constexpr std::array<LineOffset, 4> lineOffsets{
        LineOffset::Line0,
        LineOffset::Line1,
        LineOffset::Line2,
        LineOffset::Line3,
    };

    if(line >= lines_)
        line = lines_ - 1;

    const uint8_t address = column + lineOffsets[line];

    sendCmd(Command::DDRAMaddress | address);
}

void Lcd::clear() const
{
    sendCmd(Command::ClearDisplay);
    // TODO DELAY
    HAL_Delay(1);
}

void Lcd::displayOn()
{
    setFlag(displayCtrlConfig_, DisplayCtrlFlag::DisplayState);
}

void Lcd::displayOff()
{
    unsetFlag(displayCtrlConfig_, DisplayCtrlFlag::DisplayState);
}

void Lcd::cursorOn()
{
    setFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorState);
}

void Lcd::cursorOff()
{
    unsetFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorState);
}

void Lcd::cursorBlinking()
{
    setFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorBlink);
}

void Lcd::cursorNotBlinking()
{
    unsetFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorBlink);
}

void Lcd::shiftModeDisplay()
{
    setFlag(entryModeConfig_, EntryModeFlag::ShiftType);
}

void Lcd::shiftModeCursor()
{
    unsetFlag(entryModeConfig_, EntryModeFlag::ShiftType);
}

void Lcd::shiftDirectionLeft()
{
    setFlag(entryModeConfig_, EntryModeFlag::ShiftDirection);
}

void Lcd::shiftDirectionRight()
{
    unsetFlag(entryModeConfig_, EntryModeFlag::ShiftDirection);
}

void Lcd::setFlag(uint8_t& config, uint8_t flag)
{
    config |= flag;
    sendCmd(config);
}

void Lcd::unsetFlag(uint8_t& config, uint8_t flag)
{
    config &= ~flag;
    sendCmd(config);
}

void Lcd::sendCmd(uint8_t byte) const
{
    send(byte, DataSendingFlag::Backlight);
}

void Lcd::sendData(uint8_t data) const
{
    send(data, DataSendingFlag::Register | DataSendingFlag::Backlight);
}

void Lcd::send(uint8_t byte, uint8_t flags) const
{
    // since I2C requires 4 bit interface, bytes of data need to be split into nibbles
    transmitNibble(bitwise::nibbleHigh(byte) | flags);
    transmitNibble((bitwise::nibbleLow(byte) << 4) | flags);
}

void Lcd::transmitNibble(uint8_t nibble) const
{
    // the data needs to be transmitted twice
    // first time with PULSE flag, second time without it
    uint8_t data = nibble | DataSendingFlag::Pulse;
    i2c_->transmit(slaveAddress_, &data, 1);
    // TODO DELAY > 450 ns

    data = nibble & ~DataSendingFlag::Pulse;
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

    std::sprintf(lineBuffer.data(), "    %s %'.02d/%'.02d/%'.02d", day_names[rtcBuffers.weekday],
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

void Lcd::displayTaskFunction(void *args)
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
            lcd.printLine(i, lineBuffers[i].data());

        os::Task::delay(980);
    }
}

}