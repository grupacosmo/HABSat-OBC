//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef RCC_SYS_SENSOR_H
#define RCC_SYS_SENSOR_H


#include <stm32f4xx_hal.h>

#define	BME280_DIG_T1		0x88
#define	BME280_DIG_T2		0x8A
#define	BME280_DIG_T3		0x8C

#define	BME280_CONFIG			0xF5
#define	BME280_TEMPDATA			0xFA


class Sensor {
public:
    void init(SPI_HandleTypeDef *spi_handler, uint8_t temperature_resolution, uint8_t pressure_oversampling, uint8_t huminidity_oversampling, uint8_t mode);
    void sensor_set_config(uint8_t standby_time, uint8_t filter);

    float read_temperature(void);

    uint32_t read24(uint8_t addr);

    uint16_t read_16_le(uint8_t addr);

    uint16_t read_16(uint8_t addr);

    void write_8(uint8_t address, uint8_t data);

    uint8_t read_temperature_a(float *temperature);
};


#endif //RCC_SYS_SENSOR_H
