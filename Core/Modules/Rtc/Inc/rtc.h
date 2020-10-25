//
// Created by Natalia Bidzinska on 23.09.2020.
//

#ifndef RCC_SYS_RTC_H
#define RCC_SYS_RTC_H

#include "stm32f4xx.h"
#include "os_task.h"

/**
 * RTC DS1307 library for STM32F4xx.
 */
class Rtc
{
public:
    /**
     * RTC's constructor.
     * @param i2c_handle  Pointer to an I2C handle.
     * @param address     RTC address
     */
    Rtc(I2C_HandleTypeDef* i2c_handle, uint8_t address);

    /**
     * Initializes RTC.
     */
    void init() const;

    /**
     * Sets the clock.
     *
     * Date is saved in the RTC memory.
     * You should use this only once and then remove the function call from the source code.
     * Otherwise clock would start measuring from the given date every time the circuit is connected to the power.
     *
     * @param second
     * @param minute
     * @param hour      24 hour format
     * @param weekday   1 for Sunday, ..., 7 for Saturday
     * @param day
     * @param month
     * @param year      Two last digits of the year
     */
    void set_time_date(const uint8_t second, const uint8_t minute, const uint8_t hour, const uint8_t weekday,
                       const uint8_t day, const uint8_t month, const uint8_t year) const;

    /**
     * Puts the date from RTC to object's date_time structure.
     */
    void get_time_date();

    /**
     * Writes time from object's date_time structure in HH:MM:SS format to a given string.
     * @param str
     */
    void time_info(char *str) const;

    /**
     * Writes date from object's date_time structure in NAME DD/MM/YY format to a given string.
     * @param str
     */
    void date_info(char *str) const;


private:

    /**
     * Writes given uint8_t at the end of the given string.
     * @param uint
     * @param str
     */
    void add_uint_to_string(uint8_t uint, char *str) const;

    /**
     * Writes adding_str at the end of the base_str.
     * @param base_str
     * @param adding_str
     */
    void add_txt(char *base_str, const char *adding_str) const;

    /**
     * Converts a binary-coded decimal to a decimal (base-10).
     * @param data_to_convert      Binary-coded decimal
     * @return converted decimal (base-10).
     */
    static uint8_t bcd_to_dec(const uint8_t data_to_convert);

    /**
     * Converts a decimal (base-10) to a binary-coded decimal.
     * @param data_to_convert      Decimal (base-10)
     * @return converted binary-coded decimal.
     */
    static uint8_t dec_to_bcd(const uint8_t data_to_convert);

    /**
     * Converts uint8_t to char.
     * @param uint
     * @return converted char
     */
    char uint_to_char(const uint8_t uint) const;

    /**
     * Defines RTC task.
     *
     * Updates date in object's date_time structure.
     * Prints updated date on LCD screen in two lines.
     * @param args
     */
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
