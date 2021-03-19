//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "Lcd.h"

#include <array>

#include "../Utils/bitwiseOperations.h"
#include "Obc.h"

namespace lcd {

/**
   * You can find detailed explanations of all commands here:
   * https://mil.ufl.edu/3744/docs/lcdmanual/commands.html
   */
enum Command : uint8_t {
  ClearDisplay = 0x01,  // has no flags, clears display
  CursorHome   = 0x02,  // has no flags, sets cursor to line, 0 char 0
  EntryMode    = 0x04,  // has flags, changes the mode of entering new characters
  DisplayCtrl  = 0x08,  // has flags, changes the mode of displaying
  Shift        = 0x10,  // has flags, shifts cursor or entire display
  FunctionSet  = 0x20,  // has flags, changes the function set
  CGRAMaddress = 0x40,  //
  DDRAMaddress = 0x80,  //
};

// Below are the flags of all the commands that have them

enum EntryModeFlag : uint8_t {
  ShiftType      = 0x01,
  ShiftDirection = 0x02,
};

enum DisplayCtrlFlag : uint8_t {
  CursorBlink  = 0x01,
  CursorState  = 0x02,
  DisplayState = 0x04,
};

enum ShiftFlag : uint8_t {
  Direction = 0x04,
  Type      = 0x08,
};

enum FunctionSetFlag : uint8_t {
  Font            = 0x04,
  DisplayLines    = 0x08,  // flag not set - 1 line function set, flag set - 2 line function set
  InterfaceLength = 0x10,  // flag not set - 4 bit interface, flag set - 8 bit interface
};

// Flags that are appended to the data
// therefore the lower nibble of data has to be equal 0x00
enum DataSendingFlag : uint8_t {
  Register  = 0x01,
  ReadWrite = 0x02,
  Pulse     = 0x04,
  Backlight = 0x08,
};

// Line start addresses
enum LineOffset : uint8_t {
  Line0 = 0x00,
  Line1 = 0xC0,
  Line2 = 0x94,
  Line3 = 0x54,
};

Lcd::Lcd(uint16_t lines, uint16_t lineLength, const hw::I2CBus* i2c, uint8_t slaveAddress)
    : i2c_(i2c),
      lines_(lines),
      lineLength_(lineLength),
      slaveAddress_(slaveAddress),
      displayCtrlConfig_(Command::DisplayCtrl | DisplayCtrlFlag::DisplayState),
      entryModeConfig_(Command::EntryMode | EntryModeFlag::ShiftDirection) {}

void Lcd::init() const {
  auto waitAndSendCmd = [this](uint32_t timeMs, uint8_t b) {
    HAL_Delay(timeMs);
    sendCmd(b);
  };

  waitAndSendCmd(20, Command::FunctionSet);
  waitAndSendCmd(5, Command::FunctionSet);
  waitAndSendCmd(1, Command::FunctionSet);
  if (lines_ > 1)
    waitAndSendCmd(1, Command::FunctionSet | FunctionSetFlag::DisplayLines);
  else
    waitAndSendCmd(1, Command::FunctionSet);
  waitAndSendCmd(1, displayCtrlConfig_);
  waitAndSendCmd(1, Command::ClearDisplay);
  waitAndSendCmd(1, entryModeConfig_);
}

void Lcd::printLine(const u_int16_t lineIndex, const std::string& str) const {
  setCursorPosition(lineIndex, 0);

  size_t i;
  for (i = 0; i < lineLength_ && i < str.size(); i++) {
    sendData(str[i]);
  }

  for (; i < lineLength_; i++) {
    sendData(' ');
  }

  setCursorPosition(lineIndex, str.size());
}

void Lcd::printChar(const char character) const { sendData(character); }

void Lcd::setCursorPosition(uint16_t line, const uint16_t column) const {
  static constexpr std::array<LineOffset, 4> lineOffsets{
      LineOffset::Line0,
      LineOffset::Line1,
      LineOffset::Line2,
      LineOffset::Line3,
  };

  if (line >= lines_) line = lines_ - 1;

  const uint8_t address = column + lineOffsets[line];

  sendCmd(Command::DDRAMaddress | address);
}

void Lcd::clear() const {
  sendCmd(Command::ClearDisplay);
  HAL_Delay(1);
}

void Lcd::displayOn() { setFlag(displayCtrlConfig_, DisplayCtrlFlag::DisplayState); }

void Lcd::displayOff() { unsetFlag(displayCtrlConfig_, DisplayCtrlFlag::DisplayState); }

void Lcd::cursorOn() { setFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorState); }

void Lcd::cursorOff() { unsetFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorState); }

void Lcd::cursorBlinking() { setFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorBlink); }

void Lcd::cursorNotBlinking() { unsetFlag(displayCtrlConfig_, DisplayCtrlFlag::CursorBlink); }

void Lcd::shiftModeDisplay() { setFlag(entryModeConfig_, EntryModeFlag::ShiftType); }

void Lcd::shiftModeCursor() { unsetFlag(entryModeConfig_, EntryModeFlag::ShiftType); }

void Lcd::shiftDirectionLeft() { setFlag(entryModeConfig_, EntryModeFlag::ShiftDirection); }

void Lcd::shiftDirectionRight() { unsetFlag(entryModeConfig_, EntryModeFlag::ShiftDirection); }

void Lcd::setFlag(uint8_t& config, uint8_t flag) {
  config |= flag;
  sendCmd(config);
}

void Lcd::unsetFlag(uint8_t& config, uint8_t flag) {
  config &= ~flag;
  sendCmd(config);
}

void Lcd::sendCmd(uint8_t byte) const { send(byte, DataSendingFlag::Backlight); }

void Lcd::sendData(uint8_t data) const {
  send(data, DataSendingFlag::Register | DataSendingFlag::Backlight);
}

void Lcd::send(uint8_t byte, uint8_t flags) const {
  // since I2C requires 4 bit interface, bytes of data need to be split into nibbles
  transmitNibble(bitwise::nibbleHigh(byte) | flags);
  transmitNibble((bitwise::nibbleLow(byte) << 4) | flags);
}

void Lcd::transmitNibble(uint8_t nibble) const {
  // the data needs to be transmitted twice
  // first time with PULSE flag, second time without it
  uint8_t data = nibble | DataSendingFlag::Pulse;
  i2c_->transmit(slaveAddress_, data);

  data = nibble & ~DataSendingFlag::Pulse;
  i2c_->transmit(slaveAddress_, data);
}

}  // namespace hw