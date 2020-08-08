//
// Created by vixu on 8/7/20.
//

#ifndef RCC_SYS_LCD_H
#define RCC_SYS_LCD_H

#include "stm32f4xx.h"
#include "bitwise_operations.h"
#include <array>
#include <string>
#include <type_traits>

class Lcd
{
private:

    I2C_HandleTypeDef* const hi2cx;
    const uint8_t slave_address;
    const int rows;
    const int columns;

    enum class Command : uint8_t
    {
        CLEAR = 0x01,
        SET_8_BIT_INTERFACE = 0x30,
        SET_4_BIT_INTERFACE = 0x20,
        SET_4_BIT_2_LINE_FUNCTION_SET = 0x28,
        SET_4_BIT_1_LINE_FUNCTION_SET = 0x20,
        SET_8_BIT_2_LINE_FUNCTION_SET = 0x38,
        SET_8_BIT_1_LINE_FUNCTION_SET = 0x30,
        SET_CURSOR_ON = 0x0E,
        SET_CURSOR_OFF = 0x0C,
        SET_CURSOR_BLINKING = 0x0F,
        TOGGLE_ON_OFF = 0x08,
        ENTRY_MODE = 0x06,

    };

public:
    Lcd(int rows, int columns, I2C_HandleTypeDef *hi2cx, uint8_t slave_address);
    void set_cursor(int row, int col) const;
    void clear() const;
    void print(const std::string& str) const;

private:
    void i2c_transmit(std::byte *data, uint16_t size) const;
    void send_cmd(const std::byte &cmd) const;
    void send_cmd(const Command &cmd) const;
    void send_data(const std::byte &data) const;
};


#endif //RCC_SYS_LCD_H
