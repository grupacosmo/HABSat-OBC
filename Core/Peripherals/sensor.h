//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef RCC_SYS_SENSOR_H
#define RCC_SYS_SENSOR_H


#include "../System/os_task.h"
#include "stm32f4xx.h"
#include <SPIBus.h>
#include <array>

namespace hw
{

class Sensor {
public:
    enum ConfigFlags : uint8_t {
        BME280_TEMPERATURE_16BIT      = 0x01,
        FILTER_OFF                    = 0x00,
        BME280_NORMALMODE		      = 0x03,
        BME280_STANDBY_MS_10	      = 0x06,
        BME280_PRESSURE_ULTRALOWPOWER =	0x01,
        BME280_HUMINIDITY_STANDARD	  = 0x03,
    };

    struct Buffers
    {
        float temperature = 0;
        float pressure = 0;
        float humidity = 0;
    };

public:
    /**
     * Sensor's constructor.
     * @param spi_handler Pointer to SPI handle
     */

    Sensor(SPIBus* spi);

    /**
     * Initializes Sensor.
     * Sets size for temperature, pressure and humidity resolution. Set choosen mode.
     * @param temperature_resolution
     * @param pressure_oversampling
     * @param humidity_oversampling
     * @param mode
     */
    void init(const ConfigFlags & temperature_resolution, const ConfigFlags & pressure_oversampling, const ConfigFlags & humidity_oversampling, const ConfigFlags & mode);

    /**
     * Sets sensor's configuration.
     * @param standby_time
     * @param filter
     */
    void configure(const ConfigFlags & standby_time, const ConfigFlags & filter);

    /**
     *Calls function for reading temperature, pressure and humidity
     */
    void readAll();

    /**
     * Reads temperature data from sensor in 24-bit format
     * @param temperature
     */
    float readTemperature();

    /**
     * Reads pressure data form sensor in 24-bit format
     * There is a need to readTemperature first.
     * For reading pressure, variable t_fine have to be initialized
     * (It happens in readTemperature)
     * t_fine will be used in convertion
     * @param pressure
     */
    float readPressure();

    /**
     * Reads humidity data from sensor in 16-bit format
     * There is a need to readTemperature first.
     * For reading humidity, variable t_fine have to be initialized
     * (It happens in readTemperature)
     * t_fine will be used in convertion
     * @param humidity
     */
    float readHumidity();

    /*
     * Getter for the Measure Task handle.
     */
    const os::Task &getMeasureTask() const;

    const Sensor::Buffers &getBuffers() const;

private:
    enum class Address : uint8_t;
    /**
     * Converts read data from binary to decimal.
     * Uses patterns for temperature convertion from documentation.
     * @param adc_T  read data
     * @return       converted data
     */
    float convert_data_temperature(int32_t adc_T);

    /**
    * Converts read data from binary to decimal.
    * Uses patterns for pressure convertion from documentation.
    * @param adc_P  read data
    * @return       converted data
    */
    uint32_t convert_data_pressure(int32_t adc_P);

    /**
     * Converts read data from binary to decimal.
     * Uses patterns for humidity convertion from documentation.
     * @param adc_H     read data
     * @return          converted data
     */
    uint32_t convert_data_humidity(int32_t adc_H);

    /**
     * Reads data from register addr in 24-bit format to table
     * @param address
     * @return      read data
     */
    uint32_t read24(uint8_t address);
    uint32_t read24(const Address& address);

    /**
     * Reads data from register addr in 16-bit format to table
     * @param address
     * @return      read data
     */
    uint16_t read16(const uint8_t address);
    uint16_t read16(const Address& address);

    /**
     * Reads data from register addr in 8-but format to table
     * @param address
     * @return      read data
     */
    uint8_t read8(const uint8_t address);
    uint8_t read8(const Address& address);
    /**
     * Writes data in 8-bit format to register - address
     * @param address
     * @param data
     */
    void write8(const uint8_t address, const uint8_t data);
    void write8(const Address& address, const uint8_t data);

    /**
    * Defines Sensor task.
    *
    * Reads temperature,pressure and humidity
    * Prints read data on LCD screen.
    * @param args
    */
    static void measureTaskFunction(void *args);

private:

    const ChipSelect cs{GPIOC, GPIO_PIN_3};
    Buffers buffers;

    const os::Task measure_task{"measure", 256, os::Priority::IDLE, measureTaskFunction};

    /*Handler for SPI Interface*/
    SPIBus *const spi;

    /*t_fine - carries */
    int32_t t_fine;

    enum class Address : uint8_t {
        BME280_HUM_CONTROL      = 0xF2,
        BME280_CONFIG           = 0xF5,
        BME280_TEMP_DATA        = 0xFA,
        BME280_PRESSURE_DATA    = 0xF7,
        BME280_HUMID_DATA       = 0xFD,
        BME280_CTRL_MEAS_ADDR   = 0xF4

    };

    /*Compensation parameters: temperature*/
    static constexpr size_t tSize = 3;
    const std::array<uint8_t, tSize> DIG_T = {
            0x88,
            0x8A,
            0x8C
    };
    std::array<uint16_t, tSize> t;

    /*Pressure*/
    static constexpr size_t pSize = 9;
    const std::array<uint8_t, pSize> DIG_P = {
            0x8E,
            0x90,
            0x92,
            0x94,
            0x96,
            0x98,
            0x9A,
            0x9C,
            0x9E
    };
    std::array<uint16_t, pSize> p;

    /*Humidity*/
    static constexpr size_t hSize = 6;
    const std::array<uint8_t, hSize> DIG_H = {
            0xA1,
            0xE1,
            0xE3,
            0xE4,
            0xE5,
            0xE7
    };
    std::array<uint16_t, hSize> h;
};

}

#endif //RCC_SYS_SENSOR_H
