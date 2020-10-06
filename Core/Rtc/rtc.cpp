//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "Inc/rtc.h"


Rtc::Rtc(I2C_HandleTypeDef* i2c_handle, uint8_t address)
        : m_hi2cx(i2c_handle),
        m_address(address)
{}

void Rtc::initialize() const
{
    if(__HAL_RCC_I2C3_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
}

uint8_t Rtc::bcd_to_dec(uint8_t data_to_convert)
{
    return ((data_to_convert >> 4) * 10) + (data_to_convert & 0x0F);
}

uint8_t Rtc::dec_to_bcd(uint8_t data_to_convert)
{
    return ((data_to_convert / 10) << 4) | (data_to_convert % 10);
}

void Rtc::set_time_date(uint8_t second, uint8_t minute, uint8_t hour, uint8_t weekday, uint8_t day, uint8_t month,
                        uint8_t year) const
{
    uint8_t date_to_set[7];
    date_to_set[0] = dec_to_bcd(second);
    date_to_set[1] = dec_to_bcd(minute);
    date_to_set[2] = dec_to_bcd(hour);
    date_to_set[3] = dec_to_bcd(weekday);
    date_to_set[4] = dec_to_bcd(day);
    date_to_set[5] = dec_to_bcd(month);
    date_to_set[6] = dec_to_bcd(year);

    HAL_I2C_Mem_Write(m_hi2cx, m_address << 1, 0x00, 1, date_to_set, 7, HAL_MAX_DELAY);
}

void Rtc::get_time_date()
{
    uint8_t date[] = {0, 0, 0, 0, 0, 0, 0};
    if (HAL_I2C_Mem_Read(m_hi2cx, m_address << 1, 0x00, 1, date, 7, HAL_MAX_DELAY) == HAL_OK) {
        date_time.second = bcd_to_dec(date[0]);
        date_time.minute = bcd_to_dec(date[1]);
        date_time.hour = bcd_to_dec(date[2]);
        date_time.weekday_name = bcd_to_dec(date[3]);
        date_time.day = bcd_to_dec(date[4]);
        date_time.month = bcd_to_dec(date[5]);
        date_time.year = bcd_to_dec(date[6]);
    } else {
        date_time.second = 1;
        date_time.minute = 1;
        date_time.hour = 1;
        date_time.weekday_name = 1;
        date_time.day = 1;
        date_time.month = 1;
        date_time.year = 1;
    }
}

void Rtc::add_uint_to_string(uint8_t uint, char *str) const
{
    uint8_t uint_copy = uint;

    while (*str != 0) str++;

    do {
        uint_copy /= 10;
        str++;
    } while (uint_copy != 0);

    *str-- = 0;
    do {
        *str-- = uint_to_char(uint % 10);
        uint /= 10;
    } while (uint != 0);
}

void Rtc::time_info(char *str) const
{
    add_txt(str, (char *) "      ");
    add_uint_to_string(date_time.hour, str);
    add_txt(str, (char *) ":");
    add_uint_to_string(date_time.minute, str);
    add_txt(str, (char *) ":");
    add_uint_to_string(date_time.second, str);
}

void Rtc::date_info(char *str) const
{
    uint8_t week_day_num = date_time.weekday_name;

    add_txt(str, (char *) "    ");

    switch (week_day_num) {
        case 1:
            add_txt(str, (char *) "Sun");
            break;
        case 2:
            add_txt(str, (char *) "Mon");
            break;
        case 3:
            add_txt(str, (char *) "Tue");
            break;
        case 4:
            add_txt(str, (char *) "Wed");
            break;
        case 5:
            add_txt(str, (char *) "Thu");
            break;
        case 6:
            add_txt(str, (char *) "Fri");
            break;
        case 7:
            add_txt(str, (char *) "Sat");
            break;
        default:
            add_txt(str, (char *) "Err");
            break;
    }

    add_txt(str, (char *) " ");
    add_uint_to_string(date_time.day, str);
    add_txt(str, (char *) "/");
    add_uint_to_string(date_time.month, str);
    add_txt(str, (char *) "/");
    add_uint_to_string(date_time.year, str);
}

void Rtc::add_txt(char *base_str, const char *adding_str) const
{
    while (*base_str != 0) base_str++;
    while (*adding_str != 0) {
        *base_str++ = *adding_str++;
    }
    *base_str = 0;
}

char Rtc::uint_to_char(uint8_t uint) const
{
    char offset = '0';
    return uint + offset;
}
