//
// Created by Natalia Bidzinska on 23.09.2020.
//

#ifndef RCC_SYS_RTC_H
#define RCC_SYS_RTC_H

#include "stm32f4xx.h"

//constexpr uint8_t RTC_ADDRESS = 0x68;
class Rtc
{
public:
    Rtc(I2C_HandleTypeDef* i2c_handle, uint8_t address);
    void initialize() const;
    void set_time_date(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t weekday,
            const uint8_t day, const uint8_t month, const uint8_t year) const;
    void get_time_date();
    void time_info(char *str) const;
    void date_info(char *str) const;


private:
    void add_uint_to_string(uint8_t uint, char *str) const;
    void add_txt(char *base_str, const char *adding_str) const;
    static uint8_t bcd_to_dec(const uint8_t data_to_convert);
    static uint8_t dec_to_bcd(const uint8_t data_to_convert);
    char uint_to_char(const uint8_t uint) const;
    static void rtc_task_code(void *args);

private:
    const os::Task rtc_task{"rtc_task", 128, os::Task::Priority::IDLE, rtc_task_code};

    I2C_HandleTypeDef* m_hi2cx;
    uint8_t m_address;
    struct
    {
        uint8_t second;
        uint8_t minute;
        uint8_t hour;
        uint8_t weekday_name;
        uint8_t day;
        uint8_t month;
        uint8_t year;
    } date_time;
};


#endif //RCC_SYS_RTC_H
