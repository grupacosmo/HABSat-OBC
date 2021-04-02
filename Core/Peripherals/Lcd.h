//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef HABSAT_OBC_LCD_H
#define HABSAT_OBC_LCD_H

#include <array>
#include <string>

#include "I2CBus.h"
#include "Noncopyable.h"

namespace lcd {

class Lcd : public Noncopyable {
   public:
    /**
     * @param lines        number of lines
     * @param lineLength   number of chars per line
     * @param i2c
     * @param slaveAddress
     */
    Lcd(uint16_t lines, uint16_t lineLength, const mcuBoard::I2CBus& i2c, uint8_t slaveAddress);

    /**
     * Initializes LCD hardware
     */
    void init() const;

    /**
     * Sets position of a cursor. Lines start from 0.
     * @param line
     * @param column
     */
    void setCursorPosition(uint16_t line, uint16_t column) const;

    /**
     * Clears entire line and prints text.
     * @param lineIndex
     * @param str
     */
    void printLine(uint16_t lineIndex, const std::string& str) const;

    /**
     * Prints one character at the current cursor position;
     * @param character
     */
    void printChar(char character) const;

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
     * Sets shift mode to cursor shift. Whenever new characters are written the entire screen is
     * shifting.
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
    void setFlag(uint8_t& config, uint8_t flag);
    void unsetFlag(uint8_t& config, uint8_t flag);
    void sendCmd(uint8_t byte) const;
    void sendData(uint8_t data) const;
    void send(uint8_t byte, uint8_t flags) const;
    void transmitNibble(uint8_t nibble) const;

   private:
    const mcuBoard::I2CBus& i2c_;
    const uint16_t lines_;
    const uint16_t lineLength_;
    const uint16_t slaveAddress_;

    uint8_t displayCtrlConfig_;
    uint8_t entryModeConfig_;
};

}  // namespace lcd

#endif  // HABSAT_OBC_LCD_H
