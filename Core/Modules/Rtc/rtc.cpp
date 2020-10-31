//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "Inc/rtc.h"
#include <obc.h>


Rtc::Rtc(I2C_HandleTypeDef* i2c_handle, uint8_t address)
        : m_hi2cx(i2c_handle),
        m_address(address)
{}

void Rtc::init() const
{
    if(__HAL_RCC_I2C1_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C1_CLK_ENABLE();
    }

    rtc_task.add_to_scheduler();
}

uint8_t Rtc::bcd_to_dec(const uint8_t data_to_convert)
{
    return ((data_to_convert >> 4) * 10) + (data_to_convert & 0x0F);
}

uint8_t Rtc::dec_to_bcd(const uint8_t data_to_convert)
{
    return ((data_to_convert / 10) << 4) | (data_to_convert % 10);
}

void Rtc::set_time_date(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t weekday,
        const uint8_t day, const uint8_t month,const uint8_t year) const
{
    uint8_t date_to_set[7] = {
            dec_to_bcd(second),
            dec_to_bcd(minute),
            dec_to_bcd(hour),
            dec_to_bcd(weekday),
            dec_to_bcd(day),
            dec_to_bcd(month),
            dec_to_bcd(year)
    };

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
    const char* day_names[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    add_txt(str, (char *) "    ");
    add_txt(str, day_names[week_day_num - 1]);
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

char Rtc::uint_to_char(const uint8_t uint) const
{
    char offset = '0';
    return uint + offset;
}

void Rtc::rtc_task_code(void *args)
{
    Rtc &rtc = obc.peripherals.rtc;
    Lcd &lcd = obc.peripherals.lcd;
    char time[14] = {};
    char date[16] = {};
    while(true) {
        *time = {};
        *date = {};
        rtc.get_time_date();
        rtc.date_info(date);
        rtc.time_info(time);
        lcd.print_line(1, date);
        lcd.print_line(2, time);
        os::Task::delay(700);
    }
}