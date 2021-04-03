//
// Created by Natalia Bidzinska on 23.09.2020.
//

#include "Rtc/rtc.hpp"

namespace habsat::rtc {

Rtc::Rtc(const buses::I2C& i2c, uint8_t address) : i2c_(i2c), slaveAddress_(address) {}

void Rtc::init() const {
#if HW_RTC_SET_TIME
    Buffer data;
    data.second  = HW_RTC_SECOND;
    data.minute  = HW_RTC_MINUTE;
    data.hour    = HW_RTC_HOUR;
    data.weekday = HW_RTC_WEEKDAY;
    data.day     = HW_RTC_DAY;
    data.month   = HW_RTC_MONTH;
    data.year    = HW_RTC_YEAR;

    setTimeAndDate(data);
#endif
}

auto convertBcdToDec(const uint8_t bcdData) -> uint8_t {
    return ((bcdData >> 4) * 10) + (bcdData & 0x0F);
}

auto convertDecToBcd(const uint8_t decData) -> uint8_t {
    return ((decData / 10) << 4) | (decData % 10);
}

void Rtc::setTimeAndDate(const Buffer& timeAndDate) const {
    Buffer converted;
    for (size_t i = 0; i < converted.array.size(); ++i) {
        converted.array[i] = convertDecToBcd(timeAndDate.array[i]);
    }
    i2c_.memoryWrite<0x00>(slaveAddress_, converted.array);
}

void Rtc::readTimeAndDate(Buffer& buffer) {
    if (i2c_.memoryRead<0x00>(slaveAddress_, buffer.array) == buses::Result::Ok) {
        for (auto& elem : buffer.array) {
            elem = convertBcdToDec(elem);
        }
    }
}

}  // namespace rtc