//
// Created by Natalia Bidzinska on 23.09.2020.
//

#ifndef HABSAT_OBC_RTC_H
#define HABSAT_OBC_RTC_H

#include <array>

#include "Buses/i2c.hpp"

namespace habsat::rtc {

/**
 * second 0-59
 * minute 0-59
 * hour 0-23
 * weekday 0-6 (sun, mon ... sat)
 * day 1-31
 * month 1-12
 * year (two last digits of a year)
 */
struct Buffer {
    std::array<uint8_t, 7> array{};
    uint8_t& second  = array[0];
    uint8_t& minute  = array[1];
    uint8_t& hour    = array[2];
    uint8_t& weekday = array[3];
    uint8_t& day     = array[4];
    uint8_t& month   = array[5];
    uint8_t& year    = array[6];
};

class Rtc {
   public:
    /**
     * RTC's constructor.
     * @param i2c
     * @param address    RTC address
     */
    Rtc(const buses::I2C& i2c, uint8_t address);

    /**
     * Initializes RTC hardware and task into the scheduler.
     */
    void init() const;

    /**
     * Sets the clock.
     *
     * Date is saved in the RTC memory.
     * You should use this only once and then remove the function call from the source code.
     * Otherwise clock would start measuring from the given date every time the circuit is connected
     * to the power.
     *
     * @param timeAndDate
     */
    void setTimeAndDate(const Buffer& timeAndDate) const;

    /**
     * Puts the date from RTC to buffer structure.
     */
    void readTimeAndDate(Buffer& buffer);

   private:
    const buses::I2C& i2c_;
    uint8_t slaveAddress_;
};

}  // namespace habsat::rtc

#endif  // HABSAT_OBC_RTC_H
