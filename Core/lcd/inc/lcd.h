//
// Created by vixu on 8/7/20.
//


#ifndef RCC_SYS_LCD_H
#define RCC_SYS_LCD_H

#include "stm32f4xx.h"
#include "bitwise_operations.h"
#include <array>
#include <string>

class Lcd
{
public:

    /**
     * Initializes LCD.
     *
     * @param lines         Number of lines on an LCD screen.
     * @param line_length   Number of possible characters per line.
     * @param hi2cx         I2C handle.
     * @param slave_address Slave address of an LCD.
     */
    Lcd(const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *hi2cx, const uint8_t slave_address);

    void set_cursor_pos(uint16_t line, const uint16_t column) const;
    void print_line(const uint16_t line_index, const std::string& str) const;
    void print_char(const char character);
    void clear() const;
    void display_on();
    void display_off();
    void cursor_on();
    void cursor_off();
    void cursor_blinking();
    void cursor_not_blinking();
    void shift_mode_cursor();
    void shift_mode_display();
    void shift_direction_left();
    void shift_direction_right();

private:

    /**
     * You can find explanations of all commands here:
     * https://mil.ufl.edu/3744/docs/lcdmanual/commands.html
     */
    enum class Command : uint8_t {
        CLEAR_DISPLAY   = 0x01,
        CURSOR_HOME     = 0x02,
        ENTRY_MODE      = 0x04,
        DISPLAY_CRTL    = 0x08,
        SHIFT           = 0x10,
        FUNCTION_SET    = 0x20,
        CGRAM_ADDRESS   = 0x40,
        DDRAM_ADDRESS   = 0x80,
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
        DISPLAY_LINES       = 0x08,
        INTERFACE_LENGTH    = 0x10,
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

private:

    I2C_HandleTypeDef* const m_hi2cx;
    const uint16_t m_lines;
    const uint16_t m_line_length;
    const uint8_t m_slave_address;

    std::byte m_display_ctrl_config;
    std::byte m_entry_mode_config;
};


#endif //RCC_SYS_LCD_H
