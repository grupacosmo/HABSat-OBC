//
// Created by vixu on 8/7/20.
//

#include "lcd.h"


Lcd::Lcd(Interface intf, FunctionSet set, const uint16_t lines, const uint16_t line_length,
         I2C_HandleTypeDef *const hi2cx, uint8_t slave_address)

: interface(intf), func_set(set), lines(lines), line_length(line_length), hi2cx(hi2cx), slave_address(slave_address)
{
    HAL_Delay(20);  set_interface(interface);
    HAL_Delay(5);   set_interface(interface);
    HAL_Delay(1);   set_interface(interface);
    HAL_Delay(1);   set_function_set(interface, func_set);
    HAL_Delay(1);   send_cmd(Command::TOGGLE_DISPLAY);
    HAL_Delay(1);   send_cmd(Command::CLEAR_DISPLAY);
    HAL_Delay(1);   send_cmd(Command::ENTRY_MODE);
    HAL_Delay(1);   send_cmd(Command::SET_CURSOR_ON);
}

void Lcd::i2c_transmit(std::byte *const data, const uint16_t size) const
{
    HAL_I2C_Master_Transmit(hi2cx, slave_address, reinterpret_cast<uint8_t *>(data), size, 100);
}

void Lcd::send(const std::byte& byte, const RegisterBit& rs) const
{
    const std::byte nibble_high = bitwise::nibble_high(byte);
    const std::byte nibble_low = bitwise::nibble_low(byte) << 4;
    const std::array<std::byte, 4> final_byte {
            nibble_high | std::byte{0x08} | std::byte{rs} | std::byte{EnableBit::ENABLE},
            nibble_high | std::byte{0x08} | std::byte{rs} | std::byte{EnableBit::DISABLE},
            nibble_low | std::byte{0x08} | std::byte{rs} | std::byte{EnableBit::ENABLE},
            nibble_low | std::byte{0x08} | std::byte{rs} | std::byte{EnableBit::DISABLE},
    };

    i2c_transmit(const_cast<std::byte *>(&final_byte[0]), final_byte.size());
}

void Lcd::send_cmd(std::byte byte) const
{
    send(byte, RegisterBit::COMMAND);
}

void Lcd::send_cmd(const Command &cmd) const
{
    send(std::byte{cmd}, RegisterBit::COMMAND);
}

void Lcd::send_data(const std::byte &data) const
{
    send(std::byte{data}, RegisterBit::DATA);
}

void Lcd::set_cursor_pos(uint16_t line, const uint16_t col) const
{
    const uint8_t line_3 = static_cast<uint8_t>(LineAddress::LINE_0) + line_length;
    const uint8_t line_4 = static_cast<uint8_t>(LineAddress::LINE_1) + line_length;

    const std::array<std::byte, 4> line_addresses {
            std::byte{LineAddress::LINE_0},
            std::byte{LineAddress::LINE_1},
            std::byte{line_3},
            std::byte{line_4}
    };

    if(line >= lines)
        line = lines - 1;

    const uint8_t address = col + static_cast<uint8_t>(line_addresses[line]);
    send_cmd(std::byte{address});
}

void Lcd::clear() const
{
    send_cmd(Command::CLEAR_DISPLAY);
}

void Lcd::print_line(const u_int16_t line, const std::string& str) const
{
    set_cursor_pos(line, 0);
    int i;
    for(i = 0; i < line_length && i < str.size(); i++)
    {
        send_data(std::byte{str[i]});
    }
    for(; i < line_length; i++)
    {
        send_data(std::byte{' '});
    }
    set_cursor_pos(line, str.size());

}

void Lcd::set_interface(const Lcd::Interface &intf) const
{
    send_cmd(std::byte{intf});
}

void Lcd::set_function_set(const Interface &intf, const Lcd::FunctionSet &set) const
{
    send_cmd(std::byte{intf} | std::byte{set});
}

void Lcd::send_4_bits(const std::byte &data)
{
    // TODO:
}

void Lcd::send_8_bits(const std::byte &data)
{
    // TODO:
}








