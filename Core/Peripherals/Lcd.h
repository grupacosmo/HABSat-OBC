//
// Created by Wiktor Wieclaw on 8/7/20.
//


#ifndef RCC_SYS_LCD_H
#define RCC_SYS_LCD_H

#include "I2CBus.h"
#include "Task.h"
#include "stm32f4xx.h"
#include <array>
#include <string>

namespace hw
{

class Lcd
{

public:

    /**
     * Constructs LCD.
     *
     * @param lines         Number of lines on an LCD screen.
     * @param lineLength   Number of possible characters per line.
     * @param hi2cx         I2C handle.
     * @param slaveAddress Slave address of an LCD.
     */
    Lcd(uint16_t lines, uint16_t lineLength, const I2CBus * i2c, uint8_t slaveAddress);

    /**
     * Initializes LCD hardware and adds tasks to scheduler
     */
    void init() const;

    /**
     * Sets position of a cursor. Lines start from 0.
     * @param line
     * @param column
     */
    void setCursorPosition(uint16_t line, const uint16_t column) const;

    /**
     * Clears entire line and prints text.
     * @param lineIndex
     * @param str
     */
    void printLine(const uint16_t lineIndex, const std::string& str) const;

    /**
     * Prints one character at the current cursor position;
     * @param character
     */
    void printChar(const char character) const;

    /**
     * Clears entire display
     */
    void clear() const;

    /**
     * Turns on the display
     */
    void displayOn();

    /**
     * Turns off the display
     */
    void displayOff();

    /**
     * Makes the cursor visible
     */
    void cursorOn();

    /**
     * Makes the cursor invisible
     */
    void cursorOff();

    /**
     * Sets cursor into blinking mode
     */
    void cursorBlinking();

    /**
    * Sets cursor into not blinking mode
    */
    void cursorNotBlinking();

    /**
     * Sets shift mode to cursor shift. Whenever new characters are written the cursor is shifting.
     */
    void shiftModeCursor();

    /**
    * Sets shift mode to cursor shift. Whenever new characters are written the entire screen is shifting.
    */
    void shiftModeDisplay();

    /**
     * Sets shift direction to left.
     */
    void shiftDirectionLeft();

    /**
     * Sets shift direction to right.
     */
    void shiftDirectionRight();

private:

    /**
     * You can find detailed explanations of all commands here:
     * https://mil.ufl.edu/3744/docs/lcdmanual/commands.html
     */
    enum Command : uint8_t {
        ClearDisplay = 0x01, // has no flags, clears display
        CursorHome   = 0x02, // has no flags, sets cursor to line, 0 char 0
        EntryMode    = 0x04, // has flags, changes the mode of entering new characters
        DisplayCtrl  = 0x08, // has flags, changes the mode of displaying
        Shift        = 0x10, // has flags, shifts cursor or entire display
        FunctionSet  = 0x20, // has flags, changes the function set
        CGRAMaddress = 0x40, //
        DDRAMaddress = 0x80, //
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
        DisplayLines    = 0x08, // flag not set - 1 line function set, flag set - 2 line function set
        InterfaceLength = 0x10, // flag not set - 4 bit interface, flag set - 8 bit interface
    };

    // Flags that are appended to the data
    // therefore the lower nibble of data has to be equal 0x00
    // TODO provide explanations for those flags
    enum DataSendingFlag : uint8_t {
        Register = 0x01,
        ReadWrite = 0x02,
        Pulse = 0x04,
        Backlight = 0x08,
    };

    // Line start addresses
    enum LineOffset : uint8_t {
        Line0 = 0x00,
        Line1 = 0xC0,
        Line2 = 0x94,
        Line3 = 0x54,
    };

private:
    void setFlag(uint8_t& config, uint8_t flag);
    void unsetFlag(uint8_t& config, uint8_t flag);
    void sendCmd(uint8_t byte) const;
    void sendData(uint8_t data) const;
    void send(uint8_t byte, uint8_t flags) const;
    void transmitNibble(uint8_t nibble) const;

private:
    const I2CBus * const i2c_;
    const uint16_t lines_;
    const uint16_t lineLength_;
    const uint16_t slaveAddress_;

    uint8_t displayCtrlConfig_;
    uint8_t entryModeConfig_;
};

}

#endif //RCC_SYS_LCD_H
