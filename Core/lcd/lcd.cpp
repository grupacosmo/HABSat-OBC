//
// Created by vixu on 8/7/20.
//

#include "lcd.h"

// TODO:
// SWAP HAL_Delay() FUNCTION FOR A DELAY FUNCTION THAT ACCEPTS MICROSECONDS AS AN ARGUMENT
// FIND MISSING DELAYS
// CHECK DATASHEET AND ARDUINO I2C LIQUID CRYSTAL LIB FOR ACCURATE DELAY USAGE






// public:

Lcd::Lcd(const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *const hi2cx, uint8_t slave_address)
: m_lines(lines), m_line_length(line_length), m_hi2cx(hi2cx), m_slave_address(slave_address)
{
    // TODO: DELAYS, FILES ALREADY MADE IN "Core/utils"

    const std::byte interface_4bit{Command::FUNCTION_SET};

    std::byte display_lines_flag{NOFLAG};

    if(lines > 1)
        display_lines_flag = std::byte{FunctionSetFlag::DISPLAY_LINES};

    HAL_Delay(20);  send_cmd(interface_4bit);
    HAL_Delay(5);   send_cmd(interface_4bit);
    HAL_Delay(1);   send_cmd(interface_4bit);
    HAL_Delay(1);   send_cmd(interface_4bit | display_lines_flag);
    HAL_Delay(1);   send_cmd(std::byte{Command::DISPLAY_CRTL} | std::byte{DisplayCtrlFlag::DISPLAY_STATE});
    HAL_Delay(1);   send_cmd(Command::CLEAR_DISPLAY);
    HAL_Delay(1);   send_cmd(std::byte{Command::ENTRY_MODE} | std::byte{EntryModeFlag::SHIFT_DIRECTION});

}

void Lcd::print_line(const u_int16_t line_index, const std::string& str) const
{
    set_cursor_pos(line_index, 0);

    int i;
    for(i = 0; i < m_line_length && i < str.size(); i++)
    {
        send_data(std::byte{str[i]});
    }

    for(; i < m_line_length; i++)
    {
        send_data(std::byte{' '});
    }

    set_cursor_pos(line_index, str.size());
}

void Lcd::set_cursor_pos(uint16_t line, const uint16_t column) const
{
    const std::array<LineOffset, 4> line_offsets {
        LineOffset::LINE_0,
        LineOffset::LINE_1,
        LineOffset::LINE_2,
        LineOffset::LINE_3,
    };

    if(line >= m_lines)
        line = m_lines - 1;

    const uint8_t address = column + static_cast<u_int8_t>(line_offsets[line]);

    send_cmd(std::byte{Command::DDRAM_ADDRESS} | std::byte{address});
}

void Lcd::clear() const
{
    send_cmd(Command::CLEAR_DISPLAY);
    // TODO DELAY
    HAL_Delay(1);
}







// private:

void Lcd::send_cmd(const std::byte& byte) const
{
    send(byte, std::byte{DataSendingFlag::BACKLIGHT});
}

void Lcd::send_cmd(const Command &cmd) const
{
    send_cmd(std::byte{cmd});
}

void Lcd::send_data(const std::byte &data) const
{
    send(data, std::byte{DataSendingFlag::REGISTER} | std::byte{DataSendingFlag::BACKLIGHT});
}

void Lcd::send(const std::byte& byte, const std::byte& flags) const
{
    transmit_nibble(bitwise::nibble_high(byte) | flags);
    transmit_nibble((bitwise::nibble_low(byte) << 4) | flags);
}

void Lcd::transmit_nibble(const std::byte &nibble) const
{
    i2c_transmit(nibble | std::byte{DataSendingFlag::PULSE}, 1);
    // TODO DELAY > 450 ns
    i2c_transmit(nibble & ~std::byte{DataSendingFlag::PULSE}, 1);
    // TODO DELAY > 37 us
}

void Lcd::i2c_transmit(std::byte data, const uint16_t data_length) const
{
    HAL_I2C_Master_Transmit(m_hi2cx, m_slave_address, reinterpret_cast<uint8_t *>(&data), data_length, 100);
}