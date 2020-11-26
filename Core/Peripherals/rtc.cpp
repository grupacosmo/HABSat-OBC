//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "rtc.h"
#include "../OBC/obc.h"

namespace hw
{

Rtc::Rtc(const I2CBus* i2c, uint8_t address)
        : i2c_(i2c),
          slaveAddress_(address)
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
    readTimeAndDateTask_.addToScheduler();
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
    i2c_->memoryWrite<0x00>(slaveAddress_, timeAndDateToSet.data(), timeAndDateToSet.size());
}

void Rtc::readTimeAndDate()
{
    std::array<uint8_t, 7> timeAndDate;
    if (i2c_->memoryRead<0x00>(slaveAddress_, timeAndDate.data(), timeAndDate.size()) == BusResult::ok)
    {
        timeAndDateBuffer_.second = convertBcdToDec(timeAndDate[0]);
        timeAndDateBuffer_.minute = convertBcdToDec(timeAndDate[1]);
        timeAndDateBuffer_.hour = convertBcdToDec(timeAndDate[2]);
        timeAndDateBuffer_.weekday_name = convertBcdToDec(timeAndDate[3]);
        timeAndDateBuffer_.day = convertBcdToDec(timeAndDate[4]);
        timeAndDateBuffer_.month = convertBcdToDec(timeAndDate[5]);
        timeAndDateBuffer_.year = convertBcdToDec(timeAndDate[6]);
    }
}

void Rtc::readTimeAndDateTaskFunction(void *args)
{
    (void)args;
    Rtc &rtc = obc().hardware.rtc;
    while(true) {
        rtc.readTimeAndDate();
        os::Task::delay(500);
    }
}
const Rtc::Buffer &Rtc::getTimeAndDateBuffer() const
{
    return timeAndDateBuffer_;
}

}