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
    if(__HAL_RCC_I2C3_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }

#if HW_RTC_SET_TIME
    set_time_date(
        HW_RTC_SECOND,
        HW_RTC_MINUTE,
        HW_RTC_HOUR,
        HW_RTC_WEEKDAY,
        HW_RTC_DAY,
        HW_RTC_MONTH,
        HW_RTC_YEAR);
#endif
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
    std::array<uint8_t, 7> date{};
    if (HAL_I2C_Mem_Read(m_hi2cx, m_address << 1, 0x00, 1, date.data(), 7, HAL_MAX_DELAY) == HAL_OK) {
        date_time.second = bcd_to_dec(date[0]);
        date_time.minute = bcd_to_dec(date[1]);
        date_time.hour = bcd_to_dec(date[2]);
        date_time.weekday_name = bcd_to_dec(date[3]);
        date_time.day = bcd_to_dec(date[4]);
        date_time.month = bcd_to_dec(date[5]);
        date_time.year = bcd_to_dec(date[6]);
    }
}

void Rtc::rtc_task_code(void *args)
{
    (void)args;
    Rtc &rtc = obc.peripherals.rtc;
    while(true) {
        rtc.get_time_date();
        os::Task::delay(500);
    }
}
const Rtc::TimeDay &Rtc::getDateTime() const
{
    return date_time;
}
