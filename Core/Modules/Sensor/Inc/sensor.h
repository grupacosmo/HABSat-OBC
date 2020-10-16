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

    Sensor(SPI_HandleTypeDef* spi_handler);

    void init(const InitConfigFlags& temperature_resolution, const InitConfigFlags& pressure_oversampling, const InitConfigFlags& humidity_oversampling, const InitConfigFlags& mode);

    void sensor_set_config(const InitConfigFlags& standby_time, const InitConfigFlags& filter);

    void read_all(float &temperature, float &pressure, float &humidity);

    void read_temperature(float& temperature);

    float convert_data_temperature(int32_t adc_T);

    void read_pressure(float &pressure);

    int32_t convert_data_pressure(int32_t adc_P);

    void read_humidity(float &humidity);

    int32_t convert_data_humidity(int32_t adc_H);

    uint32_t read24(uint8_t addr);

    uint16_t read_16_combine(uint8_t addr);

    uint16_t read_16(uint8_t addr);

    uint8_t read_8(uint8_t addr);

    void write_8(uint8_t address, uint8_t data);

    void transmit_receive(size_t size, uint8_t *tmp);

    const os::Task &getMeasureTask() const;

private:
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
