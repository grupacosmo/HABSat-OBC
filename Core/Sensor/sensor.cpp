//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "Inc/sensor.h"

uint16_t t1, t2, t3;
int32_t t_fine;
SPI_HandleTypeDef *spi_h;

void Sensor::init(SPI_HandleTypeDef *spi_handler, uint8_t temperature_resolution, uint8_t pressure_oversampling, uint8_t huminidity_oversampling, uint8_t mode){
    spi_h = spi_handler;
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // pull the pin low
    HAL_Delay(5);
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // pull the pin high

    t1 = read_16(BME280_DIG_T1);
    t2 = read_16(BME280_DIG_T2);
    t3 = read_16(BME280_DIG_T3);

}

void Sensor::sensor_set_config(uint8_t standby_time, uint8_t filter)
{
    write_8(BME280_CONFIG, (uint8_t) (((standby_time & 0x7) << 5) | ((filter & 0x7) << 2)) & 0xFC);
}

uint8_t Sensor::read_temperature_a(float* temperature)
{
    *temperature = read_temperature();
    if(*temperature == -99)
        return -1;
    return 0;
}

float Sensor::read_temperature(void)
{
    int32_t var1, var2;
    int32_t adc_T = read24(BME280_TEMPDATA);
    if (adc_T == 0x800000)
        return -99;

    adc_T >>= 4;

    var1  = ((((adc_T>>3) - ((int32_t)t1 <<1))) *
             ((int32_t)t2)) >> 11;

    var2  = (((((adc_T>>4) - ((int32_t)t1)) *
               ((adc_T>>4) - ((int32_t)t1))) >> 12) *
             ((int32_t)t3)) >> 14;

    t_fine = var1 + var2;

    float T  = (t_fine * 5 + 128) >> 8;

    return T/100;


}
void Sensor::write_8(uint8_t address, uint8_t data)
{
    uint8_t tmp[2];
	tmp[0] = address;
	tmp[0] &= ~(1<<7);
	tmp[1] = data;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 2, 10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

}
uint16_t Sensor::read_16_le(uint8_t addr)
{
    uint16_t tmp;

    tmp = read_16(addr);
    return (tmp >> 8) | (tmp << 8);
}

uint16_t Sensor::read_16(uint8_t addr) {
    uint8_t tmp[3];
    tmp[0] = addr;
    tmp[0] |= (1 << 7);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 3, 10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    return ((tmp[1] << 8) | tmp[2]);
}

uint32_t Sensor::read24(uint8_t addr)
{
    uint8_t tmp[4];
    tmp[0] = addr;
    tmp[0] |= (1<<7);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 3, 10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    return ((tmp[1] << 16) | tmp[2] << 8 | tmp[3]);
}