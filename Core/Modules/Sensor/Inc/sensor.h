//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef RCC_SYS_SENSOR_H
#define RCC_SYS_SENSOR_H


#include "stm32f4xx.h"
#include "os_task.h"


class Sensor {
public:
    enum class InitConfigFlags : uint8_t {
        BME280_TEMPERATURE_16BIT      = 1,
        FILTER_OFF                    = 0,
        BME280_NORMALMODE		      = 3,
        BME280_STANDBY_MS_10	      = 6,
        BME280_PRESSURE_ULTRALOWPOWER =	1,
        BME280_HUMINIDITY_STANDARD	  = 3,
    };

    /**
     * Sensor's constructor.
     * @param spi_handler Pointer to SPI handle
     */

    Sensor(SPI_HandleTypeDef* spi_handler);

    /**
     * Initializes Sensor.
     * Sets size for temperature, pressure and humidity resolution. Set choosen mode.
     * @param temperature_resolution
     * @param pressure_oversampling
     * @param humidity_oversampling
     * @param mode
     */
    void init(const InitConfigFlags& temperature_resolution, const InitConfigFlags& pressure_oversampling, const InitConfigFlags& humidity_oversampling, const InitConfigFlags& mode);

    /**
     * Sets sensor's configuration.
     * @param standby_time
     * @param filter
     */
    void sensor_set_config(const InitConfigFlags& standby_time, const InitConfigFlags& filter);

    /**
     *Calls function for reading temperature, pressure and humidity
     * @param temperature
     * @param pressure
     * @param humidity
     */
    void read_all(float &temperature, float &pressure, float &humidity);

    /**
     * Reads temperature data from sensor in 24-bit format
     * @param temperature
     */
    void read_temperature(float& temperature);

    /**
     * Reads pressure data form sensor in 24-bit format
     * There is a need to read_temperature first.
     * For reading pressure, variable t_fine have to be initialized
     * (It happens in read_temperature)
     * t_fine will be used in convertion
     * @param pressure
     */
    void read_pressure(float &pressure);

    /**
     * Reads humidity data from sensor in 16-bit format
     * There is a need to read_temperature first.
     * For reading humidity, variable t_fine have to be initialized
     * (It happens in read_temperature)
     * t_fine will be used in convertion
     * @param humidity
     */
    void read_humidity(float &humidity);

    /*
     * Getter for the Measure Task handle.
     */
    const os::Task &getMeasureTask() const;
private:
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
    int32_t convert_data_pressure(int32_t adc_P);

    /**
     * Converts read data from binary to decimal.
     * Uses patterns for humidity convertion from documentation.
     * @param adc_H     read data
     * @return          converted data
     */
    int32_t convert_data_humidity(int32_t adc_H);

    /**
     * Reads data from register addr in 24-bit format to table
     * @param addr
     * @return      read data
     */
    uint32_t read24(uint8_t addr);

    /**
     * Combines two bytes by swapping their places
     * @param addr
     * @return      combined data
     */
    uint16_t read_16_combine(uint8_t addr);

    /**
     * Reads data from register addr in 16-bit format to table
     * @param addr
     * @return      read data
     */
    uint16_t read_16(uint8_t addr);

    /**
     * Reads data from register addr in 8-but format to table
     * @param addr
     * @return      read data
     */
    uint8_t read_8(uint8_t addr);

    /**
     * Writes data in 8-bit format to register - address
     * @param address
     * @param data
     */
    void write_8(uint8_t address, uint8_t data);

    /**
     * Writes data in 8-bit format to register - tmp[0]
     * Then reads data to tmp (from tmp[1] to tmp[size-1])
     * @param size  size of tmp
     * @param tmp   table of uint8
     */
    void transmit_receive(size_t size, uint8_t *tmp);

    /**
    * Defines Sensor task.
    *
    * Reads temperature,pressure and humidity
    * Prints read data on LCD screen.
    * @param args
    */
    static void measure_task_function(void *args);

private:

    const os::Task measure_task{"measure", 256, os::Task::Priority::IDLE, measure_task_function};

    /*Handler for SPI Interface*/
    SPI_HandleTypeDef *spi_h;

    /*t_fine - carries */
    int32_t t_fine{};

    enum class Command : uint8_t{
        BME280_CONFIG       = 0xF5,
        BME280_TEMPDATA     = 0xFA,
        BME280_PRESSUREDATA = 0xF7,
        BME280_HUMIDDATA    = 0xFD,
        BME280_HUM_CONTROL  = 0xF2,
    };

    /*Compensation parameters: temperature*/
    const uint8_t DIG_T[3] = {
            0x88,
            0x8A,
            0x8C
    };

    uint16_t t[3]{};

    /*Pressure*/
    const uint8_t DIG_P[9] = {
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

    uint16_t p[9]{};

    /*Humidity*/
    const uint8_t DIG_H[6] = {
            0xA1,
            0xE1,
            0xE3,
            0xE4,
            0xE5,
            0xE7
    };

    uint16_t h[6]{};
};


#endif //RCC_SYS_SENSOR_H
