//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "Inc/sensor.h"



void Sensor::init(SPI_HandleTypeDef* spi_handler, uint8_t temperature_resolution, uint8_t pressure_oversampling, uint8_t humidity_oversampling, uint8_t mode){
    spi_h = spi_handler;
    uint8_t HumReg;
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low
    HAL_Delay(5);
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high

    ut1 = read_16(DIG_T1);
    ut2 = read_16(DIG_T2);
    ut3 = read_16(DIG_T3);

    p1 = read_16(DIG_P1);
    p2 = read_16(DIG_P2);
    p3 = read_16(DIG_P3);
    p4 = read_16(DIG_P4);
    p5 = read_16(DIG_P5);
    p6 = read_16(DIG_P6);
    p7 = read_16(DIG_P7);
    p8 = read_16(DIG_P8);
    p9 = read_16(DIG_P9);

    h1 = read_8(DIG_H1);
    h2 = read_16(DIG_H2);
    h3 = read_8(DIG_H3);
    h4 = ((read_8(DIG_H4) << 4 ) | (read_8(DIG_H4+1) & 0xF) );
    h5 = ((read_8(DIG_H5+1) << 4 ) | (read_8(DIG_H5) >> 4) );
    h6 = (int8_t)read_8(DIG_H6);

    HumReg = read_8(BME280_HUM_CONTROL);
    HumReg &= 0xF8;
    HumReg |= humidity_oversampling;
    write_8(BME280_HUM_CONTROL, HumReg);

    write_8(0xF4, ((temperature_resolution<<5) | (pressure_oversampling<<2) | mode));


}

void Sensor::sensor_set_config(uint8_t standby_time, uint8_t filter)
{
    write_8(BME280_CONFIG , (uint8_t) (((standby_time & 0x7) << 5) | ((filter & 0x7) << 2)) & 0xFC);
}

float Sensor::read_temperature(float& temperature)
{
    int32_t var1, var2;

    //Reading
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low
    int32_t adc_T = read24(BME280_TEMPDATA);
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high to end reading

    if (adc_T == 0x800000)
        return -1;

    adc_T >>= 4;

    var1  = ((((adc_T>>3) - ((int32_t)ut1 << 1))) * ((int32_t)ut2)) >> 11;

    var2  = (((((adc_T>>4) - ((int32_t)ut1)) * ((adc_T >> 4) - ((int32_t)ut1))) >> 12) * ((int32_t)ut3)) >> 14;

    t_fine = var1 + var2;

    float T  = (t_fine * 5 + 128) >> 8;

    temperature = T/100;

    return 0;
}

int Sensor::read_all(float& temperature, float& pressure, float& humidity){
    int64_t var1, var2, p;
    //int64_t p;

    read_temperature(temperature);

    //Pressure
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low to start reading
    int32_t adc_P = read24(BME280_PRESSUREDATA);
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high to end reading

    adc_P >>= 4;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)p6;
    var2 = var2 + ((var1*(int64_t)p5)<<17);
    var2 = var2 + (((int64_t)p4)<<35);
    var1 = ((var1 * var1 * (int64_t)p3)>>8) + ((var1 * (int64_t)p2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)p1)>>33;

    if (var1 == 0) {
        return 0;
    }
    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)p9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)p8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)p7)<<4);
    pressure = (int32_t)p/256;

    //Humidity
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low to start reading
    int32_t adc_H = read_16(BME280_HUMIDDATA);
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin low to start reading

    if (adc_H == 0x8000) // value in case humidity measurement was disabled
        return -1; //error

    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)h4) << 20) -
                    (((int32_t)h5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)h6)) >> 10) *
                      (((v_x1_u32r * ((int32_t)h3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)h2) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)h1)) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    float h = (v_x1_u32r>>12);
    humidity = h / 1024.0;

    return 0;
}
/* Returns Pressures value in Pa*/
float Sensor::read_pressure(float& pressure){

    int32_t var1, var2, p;
    float T;
    read_temperature(T); //Have to be done to get t_fine

    //Reading
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low to start reading
    int32_t adc_P = read24(BME280_PRESSUREDATA);
    adc_P = read24(BME280_PRESSUREDATA);
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high to end reading

    adc_P >>= 4;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)p6;
    var2 = var2 + ((var1*(int64_t)p5)<<17);
    var2 = var2 + (((int64_t)p4)<<35);
    var1 = ((var1 * var1 * (int64_t)p3)>>8) + ((var1 * (int64_t)p2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)p1)>>33;

    if (var1 == 0) {
        return 0;
    }
    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)p9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)p8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)p7)<<4);
    pressure = (int32_t)p/256;

    return 0;
}

void Sensor::write_8(uint8_t address, uint8_t data)
{
    uint8_t tmp[2];
	tmp[0] = address;
	tmp[0] &= ~(1<<7);
	tmp[1] = data;
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 2, 10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);

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
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 3, 10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    //return ((tmp[1] << 8) | tmp[2]);
    uint16_t temp;

    temp =(tmp[1] << 8) | tmp[2];
    return (temp >> 8) | (temp << 8);
}

uint8_t Sensor::read_8(uint8_t addr){
    uint8_t tmp[2];
    tmp[0] = addr;
    tmp[0] |= (1<<7);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 2, 10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    return tmp[1];
}

uint32_t Sensor::read24(uint8_t addr)
{
    uint8_t tmp[4];
    tmp[0] = addr;
    tmp[0] |= (1<<7);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, 3, 10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    return ((tmp[1] << 16) | tmp[2] << 8 | tmp[3]);
}