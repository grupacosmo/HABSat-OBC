//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef RCC_SYS_SENSOR_H
#define RCC_SYS_SENSOR_H


#include <stm32f4xx_hal.h>

class Sensor {
public:

    void init(SPI_HandleTypeDef *spi_handler, uint8_t temperature_resolution, uint8_t pressure_oversampling, uint8_t huminidity_oversampling, uint8_t mode);
    void sensor_set_config(uint8_t standby_time, uint8_t filter);

    float read_temperature(float* temperature);

    float read_pressure(float *pressure);

    int read_all(float *temperature, float *pressure, float *huminidity);

    uint32_t read24(uint8_t addr);

    uint16_t read_16_le(uint8_t addr);

    uint16_t read_16(uint8_t addr);

    void write_8(uint8_t address, uint8_t data);

private:

    uint8_t	BME280_CONFIG = 0xF5;
    uint8_t	BME280_TEMPDATA = 0xFA;
    uint8_t BME280_PRESSUREDATA = 0xF7;
    uint8_t BME280_HUMIDDATA=0xFD;

    /*Handler for SPI Interface*/
    SPI_HandleTypeDef *spi_h;

    /*t_fine - carries */
    int32_t t_fine;

    /*Compensation parameters: temperature*/
    uint8_t DIG_T1 = 0x88;
    uint8_t DIG_T2 = 0x8A;
    uint8_t DIG_T3 = 0x8C;

    uint16_t ut1, ut2, ut3;

    /*pressure*/

    uint8_t DIG_P1 =0x8E ;
    uint8_t DIG_P2 =0x90 ;
    uint8_t DIG_P3 =0x92 ;
    uint8_t DIG_P4 =0x94 ;
    uint8_t DIG_P5 =0x96 ;
    uint8_t DIG_P6 =0x98 ;
    uint8_t DIG_P7 =0x9A ;
    uint8_t DIG_P8 =0x9C ;
    uint8_t DIG_P9 =0x9E ;

    uint16_t p1, p2, p3, p4, p5, p6, p7, p8, p9;



};


#endif //RCC_SYS_SENSOR_H
