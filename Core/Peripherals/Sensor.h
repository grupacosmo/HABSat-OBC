//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef RCC_SYS_SENSOR_H
#define RCC_SYS_SENSOR_H


#include "osTask.h"
#include "stm32f4xx.h"
#include <SPIBus.h>
#include <array>

namespace hw
{

class Sensor {
public:

    struct Buffer
    {
        std::array<float, 3> array;
        float& temperature = array[0];
        float& pressure    = array[1];
        float& humidity    = array[2];
    };

    enum ConfigFlags : uint8_t {
        Temperature16Bit      = 0x01,
        FilterOff             = 0x00,
        NormalMode            = 0x03,
        Standby10Ms           = 0x06,
        PressureUltraLowPower =	0x01,
        HumidityStandard      = 0x03,
    };

public:
    /**
     * Sensor's constructor.
     * @param spi_handler Pointer to SPI handle
     */

    Sensor(const SPIBus* spi, ChipSelect* chipSelect);

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
    void readAll(Buffer& buffer);

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

private:

    /*Handler for SPI Interface*/
    const SPIBus *const spi_;
    ChipSelect* cs_;

    /*t_fine - carries */
    int32_t tFine_;

    enum class Address : uint8_t {
        HumControl      = 0xF2,
        Config          = 0xF5,
        TempData        = 0xFA,
        PressureData    = 0xF7,
        HumidityData    = 0xFD,
        CTRLmeasAddress = 0xF4

    };

    /*Compensation parameters: temperature*/
    static constexpr size_t tSize_ = 3;
    const std::array<uint8_t, tSize_> digT_ = {
            0x88,
            0x8A,
            0x8C
    };
    std::array<uint16_t, tSize_> t_;

    /*Pressure*/
    static constexpr size_t pSize_ = 9;
    const std::array<uint8_t, pSize_> digP_ = {
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
    std::array<uint16_t, pSize_> p_;

    /*Humidity*/
    static constexpr size_t hSize_ = 6;
    const std::array<uint8_t, hSize_> digH_ = {
            0xA1,
            0xE1,
            0xE3,
            0xE4,
            0xE5,
            0xE7
    };
    std::array<uint16_t, hSize_> h_;
};

}

#endif //RCC_SYS_SENSOR_H
