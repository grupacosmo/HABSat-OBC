//
// Created by Natalia Bidzinska on 23.09.2020.
//

#ifndef RCC_SYS_RTC_H
#define RCC_SYS_RTC_H

#include "stm32f4xx.h"

class Rtc
{
public:

    Rtc(I2C_HandleTypeDef* i2c_handle, uint8_t address);
    void initialize() const;
    void set_time_date() const;
    void get_time_date();
    void time_info(char*) const;
    void date_info(char*) const;

private:
    char* my_utoa(unsigned, char*) const;
    void add_txt(char*, char*) const;
    static uint8_t bcd_to_dec(uint8_t);
    static uint8_t dec_to_bcd(uint8_t);

private:
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
