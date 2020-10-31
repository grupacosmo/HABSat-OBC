//
// Created by Wiktor Wieclaw on 8/7/20.
//


#ifndef RCC_SYS_LCD_H
#define RCC_SYS_LCD_H

#include "stm32f4xx.h"
#include <string>
#include "os_task.h"

class Lcd
{

public:

    /**
     * Constructs LCD.
     *
     * @param lines         Number of lines on an LCD screen.
     * @param line_length   Number of possible characters per line.
     * @param hi2cx         I2C handle.
     * @param slave_address Slave address of an LCD.
     */
    Lcd(const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *hi2cx, const uint8_t slave_address);

    /**
     * Initializes LCD hardware and adds tasks to scheduler
     */
    void init() const;

    /**
     * Sets position of a cursor. Lines start from 0.
     * @param line
     * @param column
     */
    void set_cursor_pos(uint16_t line, const uint16_t column) const;

    /**
     * Clears entire line and prints text.
     * @param line_index
     * @param str
     */
    void print_line(const uint16_t line_index, const std::string& str) const;

    /**
     * Prints one character at the current cursor position;
     * @param character
     */
    void print_char(const char character) const;

    /**
     * Clears entire display
     */
    void clear() const;

    /**
     * Turns on the display
     */
    void display_on();

    /**
     * Turns off the display
     */
    void display_off();

    /**
     * Makes the cursor visible
     */
    void cursor_on();

    /**
     * Makes the cursor invisible
     */
    void cursor_off();

    /**
     * Sets cursor into blinking mode
     */
    void cursor_blinking();

    /**
    * Sets cursor into not blinking mode
    */
    void cursor_not_blinking();

    /**
     * Sets shift mode to cursor shift. Whenever new characters are written the cursor is shifting.
     */
    void shift_mode_cursor();

    /**
    * Sets shift mode to cursor shift. Whenever new characters are written the entire screen is shifting.
    */
    void shift_mode_display();

    /**
     * Sets shift direction to left.
     */
    void shift_direction_left();

    /**
     * Sets shift direction to right.
     */
    void shift_direction_right();

private:

    /**
     * You can find detailed explanations of all commands here:
     * https://mil.ufl.edu/3744/docs/lcdmanual/commands.html
     */
    enum class Command : uint8_t {
        CLEAR_DISPLAY   = 0x01, // has no flags, clears display
        CURSOR_HOME     = 0x02, // has no flags, sets cursor to line, 0 char 0
        ENTRY_MODE      = 0x04, // has flags, changes the mode of entering new characters
        DISPLAY_CRTL    = 0x08, // has flags, changes the mode of displaying
        SHIFT           = 0x10, // has flags, shifts cursor or entire display
        FUNCTION_SET    = 0x20, // has flags, changes the function set
        CGRAM_ADDRESS   = 0x40, //
        DDRAM_ADDRESS   = 0x80, //
    };

    // Below are the flags of all the commands that have them

    enum class EntryModeFlag : uint8_t {
        SHIFT_TYPE      = 0x01,
        SHIFT_DIRECTION = 0x02,
    };

    enum class DisplayCtrlFlag : uint8_t {
        CURSOR_BLINK    = 0x01,
        CURSOR_STATE    = 0x02,
        DISPLAY_STATE   = 0x04,
    };

    enum class ShiftFlag : uint8_t {
        DIRECTION   = 0x04,
        TYPE        = 0x08,
    };

    enum class FunctionSetFlag : uint8_t {
        FONT                = 0x04,
        DISPLAY_LINES       = 0x08, // flag not set - 1 line function set, flag set - 2 line function set
        INTERFACE_LENGTH    = 0x10, // flag not set - 4 bit interface, flag set - 8 bit interface
    };

    const std::byte NO_FLAG{0x00};

    // Flags that are appended to the data
    // therefore the lower nibble of data has to be equal 0x00
    // TODO provide explanations for those flags
    enum class DataSendingFlag : uint8_t {
        REGISTER    = 0x01,
        READ_WRITE  = 0x02,
        PULSE       = 0x04,
        BACKLIGHT   = 0x08,
    };

    // Line start addresses
    enum class LineOffset : uint8_t {
        LINE_0 = 0x00,
        LINE_1 = 0xC0,
        LINE_2 = 0x94,
        LINE_3 = 0x54,
    };

private:
    void set_flag(std::byte& config, const std::byte& flag);
    void unset_flag(std::byte& config, const std::byte& flag);
    void send_cmd(const std::byte& byte) const;
    void send_cmd(const Command &cmd) const;
    void send_data(const std::byte &data) const;
    void send(const std::byte& byte, const std::byte& flags) const;
    void transmit_nibble(const std::byte& nibble) const;
    inline void i2c_transmit(std::byte data, const uint16_t data_length) const;
    static void display_task_function(void *args);

private:
    const os::Task display_task{"display_task", 512, os::Task::Priority::IDLE, display_task_function};

    I2C_HandleTypeDef* const m_hi2cx;
    const uint16_t m_lines;
    const uint16_t m_line_length;
    const uint8_t m_slave_address;

    std::byte m_display_ctrl_config;
    std::byte m_entry_mode_config;
};


#endif //RCC_SYS_LCD_H
