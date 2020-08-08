//
// Created by vixu on 8/7/20.
//

#include "lcd.h"


Lcd::Lcd(const int rows, const int columns, I2C_HandleTypeDef *const hi2cx, uint8_t slave_address)
: rows(rows), columns(columns), hi2cx(hi2cx), slave_address(slave_address)
{
    // LCD initialization procedure done according to the datasheet

    HAL_Delay(20);  send_cmd(Command::SET_4_BIT_INTERFACE);
    HAL_Delay(5);   send_cmd(Command::SET_4_BIT_INTERFACE);
    HAL_Delay(1);   send_cmd(Command::SET_4_BIT_INTERFACE);
    HAL_Delay(1);   send_cmd(Command::SET_4_BIT_2_LINE_FUNCTION_SET);
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
    const std::byte cmd_high = bitwise::nibble_high(std::byte{cmd});
    const std::byte cmd_low = bitwise::nibble_low(std::byte{cmd});
    const std::array<std::byte, 4> cmd_final {
            cmd_high | std::byte{0x0C},    // en == 1, rs == 0
            cmd_high | std::byte{0x08},    // en == 0, rs == 0
            cmd_low | std::byte{0x0C},     // en == 1, rs == 0
            cmd_low | std::byte{0x08},     // en == 0. rs == 0
    };
    // HAL won't accept const pointer, hence usage of const_cast
    i2c_transmit(const_cast<std::byte *>(&cmd_final[0]), cmd_final.size());
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

void Lcd::set_cursor(const int row, const int col) const
{
    // TODO: change array name
    const std::array<std::byte, 2> arr = {std::byte{0x80}, std::byte{0xC0}};
    send_cmd(static_cast<std::byte>(col) | arr[row]);
}

void Lcd::clear() const
{
    send_cmd(Command::CLEAR);
}

void Lcd::print(const std::string& str) const
{
    for(const auto character : str)
    {
        send_data(std::byte{character});
    }
}





