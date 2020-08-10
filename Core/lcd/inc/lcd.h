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
     * @param lines         Number of lines on a LCD screen.
     * @param line_length   Number of possible characters per line.
     * @param hi2cx         I2C handle.
     * @param slave_address
     */
    Lcd(const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *hi2cx, const uint8_t slave_address);

    void set_cursor_pos(uint16_t line, const uint16_t column) const;
    void clear() const;
    void print_line(const uint16_t line_index, const std::string& str) const;

private:

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

    const std::byte NOFLAG{0x00};

    // new open
    enum class EntryModeFlag : uint8_t {
        SHIFT_DIRECTION = 1 << 1,
        SHIFT_TYPE      = 1 << 0
    };

    enum class DisplayCtrlFlag : uint8_t {
        DISPLAY_STATE   = 1 << 2,
        CURSOR_STATE    = 1 << 1,
        CURSOR_BLINK    = 1 << 0
    };

    enum class ShiftFlag : uint8_t {
        TYPE        = 1 << 3,
        DIRECTION   = 1 << 2
    };

    enum class FunctionSetFlag : uint8_t {
        INTERFACE_LENGTH    = 1 << 4,
        DISPLAY_LINES       = 1 << 3,
        FONT                = 1 << 2
    };

    enum class DataSendingFlag : uint8_t
    {
        BACKLIGHT   = 1 << 3,
        PULSE       = 1 << 2,
        READ_WRITE  = 1 << 1,
        REGISTER    = 1 << 0
    };

    enum class LineOffset : uint8_t {
        LINE_0 = 0x00,
        LINE_1 = 0xC0,
        LINE_2 = 0x94,
        LINE_3 = 0x54,
    };

private:

    inline void send_cmd(const std::byte& byte) const;
    inline void send_cmd(const Command &cmd) const;
    inline void send_data(const std::byte &data) const;
    void send(const std::byte& byte, const std::byte& flags) const;
    void transmit_nibble(const std::byte& nibble) const;
    inline void i2c_transmit(std::byte data, const uint16_t data_length) const;

private:

    I2C_HandleTypeDef* const m_hi2cx;
    const uint16_t m_lines;
    const uint16_t m_line_length;
    const uint8_t m_slave_address;
};


#endif //RCC_SYS_LCD_H
