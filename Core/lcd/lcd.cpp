//
// Created by vixu on 8/7/20.
//

#include "lcd.h"


Lcd::Lcd(Interface intf, const uint16_t lines, const uint16_t line_length, I2C_HandleTypeDef *const hi2cx,
         uint8_t slave_address)
: interface(intf), lines(lines), line_length(line_length), hi2cx(hi2cx), slave_address(slave_address)
{
    HAL_Delay(20);  set_interface(interface);
    HAL_Delay(5);   set_interface(interface);
    HAL_Delay(1);   set_interface(interface);
    HAL_Delay(1);   set_function_set(interface, FunctionSet::_2_LINE);
    HAL_Delay(1);   send_cmd(Command::TOGGLE_ON_OFF);
    HAL_Delay(1);   send_cmd(Command::CLEAR);
    HAL_Delay(1);   send_cmd(Command::ENTRY_MODE);
    HAL_Delay(1);   send_cmd(Command::SET_CURSOR_OFF);
}

void Lcd::i2c_transmit(std::byte *const data, const uint16_t size) const
{
    HAL_I2C_Master_Transmit(hi2cx, slave_address, reinterpret_cast<uint8_t *>(data), size, 100);
}

void Lcd::send_cmd(const std::byte &cmd) const
{
    const std::byte cmd_high = bitwise::nibble_high(cmd);
    const std::byte cmd_low = bitwise::nibble_low(cmd);
    const std::array<std::byte, 4> cmd_final {
            cmd_high | std::byte{0x0C},    // en == 1, rs == 0
            cmd_high | std::byte{0x08},    // en == 0, rs == 0
            cmd_low | std::byte{0x0C},     // en == 1, rs == 0
            cmd_low | std::byte{0x08},     // en == 0. rs == 0
    };
    // HAL won't accept const pointer, hence usage of const_cast
    i2c_transmit(const_cast<std::byte *>(&cmd_final[0]), cmd_final.size());
}

void Lcd::send_cmd(const Command &cmd) const
{
    send_cmd(std::byte{cmd});
}

void Lcd::send_data(const std::byte &data) const
{
    const std::byte data_high = bitwise::nibble_high(data);
    const std::byte data_low = bitwise::nibble_low(data);
    const std::array<std::byte, 4> data_final {
            data_high | std::byte{0x0D},    // en == 1, rs == 0
            data_high | std::byte{0x09},    // en == 0, rs == 0
            data_low | std::byte{0x0D},     // en == 1, rs == 0
            data_low | std::byte{0x09},     // en == 0. rs == 0
    };
    // HAL won't accept const pointer, hence usage of const_cast
    i2c_transmit(const_cast<std::byte *>(&data_final[0]), data_final.size());
}

void Lcd::set_cursor(int line, const int col) const
{
    const uint8_t line_3 = static_cast<uint8_t>(LineBeginning::LINE_0) + line_length;
    const uint8_t line_4 = static_cast<uint8_t>(LineBeginning::LINE_1) + line_length;

    const std::array<std::byte, 4> offsets {
            std::byte{LineBeginning::LINE_0},
            std::byte{LineBeginning::LINE_1},
            std::byte{line_3},
            std::byte{line_4}
    };

    if(line > lines)
        line = lines - 1;

    send_cmd(static_cast<std::byte>(col) | offsets[line]);
}

void Lcd::clear() const
{
    send_cmd(Command::CLEAR);
}

void Lcd::print_line(const u_int16_t line, const std::string& str) const
{
    set_cursor(line, 0);
    int i;
    for(i = 0; i < line_length && i < str.size(); i++)
    {
        send_data(std::byte{str[i]});
    }
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








