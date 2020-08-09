//
// Created by vixu on 8/7/20.
//

/*  # Notes for future documentation
 *  * There are two registers in every character lcd Data and Command
 *
 *
 */

#ifndef RCC_SYS_LCD_H
#define RCC_SYS_LCD_H

#include "stm32f4xx.h"
#include "bitwise_operations.h"
#include <array>
#include <string>

class Lcd
{
public:

    enum class Interface : uint8_t
    {
        _8_BIT = 0x30,
        _4_BIT = 0x20,
    };

    enum class FunctionSet : uint8_t
    {
        _2_LINE = 0x08,
        _1_LINE = 0x00,
    };

public:
    Lcd(Interface interface, FunctionSet set, uint16_t lines, uint16_t line_length, I2C_HandleTypeDef *hi2cx,
        uint8_t slave_address);
    void set_cursor_pos(uint16_t line, uint16_t col) const;
    void clear() const;
    void print_line(const uint16_t line, const std::string& str) const;

private:

    enum class Command : uint8_t
    {
        CLEAR_DISPLAY = 0x01,
        SET_CURSOR_ON = 0x0E,
        SET_CURSOR_OFF = 0x0C,
        SET_CURSOR_BLINKING = 0x0F,
        TOGGLE_DISPLAY = 0x08,
        ENTRY_MODE = 0x06, // TODO: check
    };

    enum class LineAddress : uint8_t
    {
        LINE_0 = 0x80,
        LINE_1 = 0xC0,
    };

    enum class RegisterBit : uint8_t
    {
        DATA = 0x01,
        COMMAND = 0x00,
    };

    enum class EnableBit : uint8_t
    {
        DISABLE = 0x00,
        ENABLE = 0x04,
    };

    I2C_HandleTypeDef* const hi2cx;
    const uint16_t lines;
    const uint16_t line_length;
    const uint8_t slave_address;
    const Interface interface;
    const FunctionSet func_set;

private:

    inline void i2c_transmit(std::byte *data, uint16_t size) const;
    void send(const std::byte& byte, const RegisterBit& rs) const;
    inline void send_cmd(std::byte byte) const;
    inline void send_cmd(const Command &cmd) const;
    inline void send_data(const std::byte &data) const;
    inline void set_interface(const Interface &interface) const;
    inline void set_function_set(const Interface &intf, const FunctionSet &set) const;
    void send_4_bits(const std::byte& data);
    void send_8_bits(const std::byte& data);
};


#endif //RCC_SYS_LCD_H
