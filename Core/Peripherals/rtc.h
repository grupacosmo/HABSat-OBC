//
// Created by Natalia Bidzinska on 23.09.2020.
//

#ifndef RCC_SYS_RTC_H
#define RCC_SYS_RTC_H

#include "os_task.h"
#include "I2CBus.h"
#include "stm32f4xx.h"
#include <array>

namespace hw
{

struct TimeAndDate {
    std::array<uint8_t, 7> array;
    uint8_t& second  = array[0];
    uint8_t& minute  = array[1];
    uint8_t& hour    = array[2];
    uint8_t& weekday = array[3];
    uint8_t& day     = array[4];
    uint8_t& month   = array[5];
    uint8_t& year    = array[6];
};

/**
 * RTC DS1307 library for STM32F4xx.
 */
class Rtc
{
public:
    /**
     * RTC's constructor.
     * @param i2cHandle  Pointer to an I2C handle.
     * @param address     RTC address
     */
    Rtc(const I2CBus* i2c, uint8_t address);

    /**
     * Initializes RTC hardware and task into the scheduler.
     */
    void init() const;

    /**
     * Sets the clock.
     *
     * Date is saved in the RTC memory.
     * You should use this only once and then remove the function call from the source code.
     * Otherwise clock would start measuring from the given date every time the circuit is connected to the power.
     *
     * @param timeAndDate
     */
    void setTimeAndDate(const TimeAndDate& timeAndDate) const;

    /**
     * Puts the date from RTC to object's date_time structure.
     */
    void readTimeAndDate();

    /**
     * Getter for timeAndDateBuffer.
     * @return
     */
    const TimeAndDate &getTimeAndDateBuffer() const;

private:

    /**
     * Converts a binary-coded decimal to a decimal (base-10).
     * @param bcdData      Binary-coded decimal
     * @return converted decimal (base-10).
     */
    static uint8_t convertBcdToDec(const uint8_t bcdData);

    /**
     * Converts a decimal (base-10) to a binary-coded decimal.
     * @param decData      Decimal (base-10)
     * @return converted binary-coded decimal.
     */
    static uint8_t convertDecToBcd(const uint8_t decData);

    /**
     * Defines RTC task.
     *
     * Updates date in object's date_time structure.
     * Prints updated date on LCD screen in two lines.
     * @param args
     */
    static void readTimeAndDateTaskFunction(void *args);


private:
    const os::Task readTimeAndDateTask_{"rtc_task",
                                         128,
                                         os::Priority::Idle,
                                         readTimeAndDateTaskFunction};

    const I2CBus *const i2c_;
    uint8_t slaveAddress_;
    TimeAndDate buffer_;
};

}

#endif //RCC_SYS_RTC_H
