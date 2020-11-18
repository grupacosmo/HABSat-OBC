//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "rtc.h"
#include "../OBC/obc.h"

namespace hw
{

Rtc::Rtc(const I2CBus* i2c, uint8_t address)
        : i2c(i2c),
        m_address(address)
{}

void Rtc::init() const
{
#if HW_RTC_SET_TIME
    setTimeAndDate(
        HW_RTC_SECOND,
        HW_RTC_MINUTE,
        HW_RTC_HOUR,
        HW_RTC_WEEKDAY,
        HW_RTC_DAY,
        HW_RTC_MONTH,
        HW_RTC_YEAR);
#endif
    m_readTimeAndDateTask.addToScheduler();
}

uint8_t Rtc::convertBcdToDec(const uint8_t bcdData)
{
    return ((bcdData >> 4) * 10) + (bcdData & 0x0F);
}

uint8_t Rtc::convertDecToBcd(const uint8_t decData)
{
    return ((decData / 10) << 4) | (decData % 10);
}

void Rtc::setTimeAndDate(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t weekday,
        const uint8_t day, const uint8_t month,const uint8_t year) const
{
    std::array<uint8_t, 7> timeAndDateToSet = {
            convertDecToBcd(second),
            convertDecToBcd(minute),
            convertDecToBcd(hour),
            convertDecToBcd(weekday),
            convertDecToBcd(day),
            convertDecToBcd(month),
            convertDecToBcd(year)
    };
    i2c->memoryWrite<0x00>(m_address, timeAndDateToSet.data(), timeAndDateToSet.size());
}

void Rtc::readTimeAndDate()
{
    std::array<uint8_t, 7> timeAndDate;
    if (i2c->memoryRead<0x00>(m_address, timeAndDate.data(), timeAndDate.size()) == BusResult::ok)
    {
        m_timeAndDateBuffer.second = convertBcdToDec(timeAndDate[0]);
        m_timeAndDateBuffer.minute = convertBcdToDec(timeAndDate[1]);
        m_timeAndDateBuffer.hour = convertBcdToDec(timeAndDate[2]);
        m_timeAndDateBuffer.weekday_name = convertBcdToDec(timeAndDate[3]);
        m_timeAndDateBuffer.day = convertBcdToDec(timeAndDate[4]);
        m_timeAndDateBuffer.month = convertBcdToDec(timeAndDate[5]);
        m_timeAndDateBuffer.year = convertBcdToDec(timeAndDate[6]);
    }
}

void Rtc::readTimeAndDateTaskFunction(void *args)
{
    (void)args;
    Rtc &rtc = obc.hardware.rtc;
    while(true) {
        rtc.readTimeAndDate();
        os::Task::delay(500);
    }
}
const Rtc::Buffer &Rtc::getTimeAndDateBuffer() const
{
    return m_timeAndDateBuffer;
}

}