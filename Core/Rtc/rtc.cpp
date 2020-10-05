//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "Inc/rtc.h"


Rtc::Rtc(I2C_HandleTypeDef* i2c_handle, uint8_t address)
        : m_hi2cx(i2c_handle),
        m_address(address)
{
    date_time.second = dec_to_bcd(0);
    date_time.minute = dec_to_bcd(26);
    date_time.hour = dec_to_bcd(19);
    date_time.weekday_name = dec_to_bcd(7);
    date_time.day = dec_to_bcd(26);
    date_time.month = dec_to_bcd(9);
    date_time.year = dec_to_bcd(20);
}

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

void Rtc::set_time_date() const
{
    uint8_t date_to_set[7];
    date_to_set[0] = date_time.second;
    date_to_set[1] = date_time.minute;
    date_to_set[2] = date_time.hour;
    date_to_set[3] = date_time.weekday_name;
    date_to_set[4] = date_time.day;
    date_to_set[5] = date_time.month;
    date_to_set[6] = date_time.year;

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
        date_time.minute = 2;
        date_time.hour = 3;
        date_time.weekday_name = 4;
        date_time.day = 5;
        date_time.month = 6;
        date_time.year = 7;
    }
}

char *Rtc::my_utoa(unsigned value_to_convert, char *converted_string) const
{
    char *string_copy = converted_string;
    unsigned value_copy = value_to_convert;
    char temp;

    do {
        value_copy /= 10;
        string_copy++;
    } while (value_copy != 0);
    *string_copy-- = 0;
    do {
        temp = value_to_convert % 10;
        value_to_convert /= 10;
        temp += '0';
        *string_copy-- = temp;
    } while (value_to_convert != 0);
    return string_copy;
}

void Rtc::time_info(char *str) const
{
    char temp_str[32] = {};
    add_txt(str, (char *) "      ");
    my_utoa(date_time.hour, temp_str);
    add_txt(str, temp_str);
    add_txt(str, (char *) ":");
    my_utoa(date_time.minute, temp_str);
    add_txt(str, temp_str);
    add_txt(str, (char *) ":");
    my_utoa(date_time.second, temp_str);
    add_txt(str, temp_str);
}

void Rtc::date_info(char *str) const
{
    char temp_str[32] = {};
    uint8_t week_day_num = date_time.weekday_name;
    char *week_day_name = {};

    add_txt(str, (char *) "    ");

    switch (week_day_num) {
        case 1:
            week_day_name = (char *) "Sun";
            break;
        case 2:
            week_day_name = (char *) "Mon";
            break;
        case 3:
            week_day_name = (char *) "Tue";
            break;
        case 4:
            week_day_name = (char *) "Wed";
            break;
        case 5:
            week_day_name = (char *) "Thu";
            break;
        case 6:
            week_day_name = (char *) "Fri";
            break;
        case 7:
            week_day_name = (char *) "Sat";
            break;
        default:
            week_day_name = (char *) "Err";
            break;
    }
    add_txt(str, week_day_name);
    add_txt(str, (char *) " ");
    my_utoa(date_time.day, temp_str);
    add_txt(str, temp_str);
    add_txt(str, (char *) "/");
    my_utoa(date_time.month, temp_str);
    add_txt(str, temp_str);
    add_txt(str, (char *) "/");
    my_utoa(date_time.year, temp_str);
    add_txt(str, temp_str);
}

void Rtc::add_txt(char *base_str, char *adding_str) const
{
    while (*base_str != 0) base_str++;
    while (*adding_str != 0) {
        *base_str++ = *adding_str++;
    }
    *base_str = 0;
}


