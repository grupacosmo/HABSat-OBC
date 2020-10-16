//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "sensor.h"
#include "obc.h"


// TODO: consty wszedzie gdzie sie da do parametrow
// TODO: consty do stalych wewnatrz funkcji
// TODO: polaczyc krotkie linijki ze soba tam gdzie sie da
// TODO: powtarzajacy sie kod ubrac w prywatne metody
// TODO: kod znajdujacy w duzych funkcjachh podzielic na czesci i ubrac w swoje wlasne metody prywatne
// TODO: lepsze, dluzsze nazwy dla zmiennych

Sensor::Sensor(SPI_HandleTypeDef* spi_handler) : spi_h(spi_handler){}

void Sensor::init(const InitConfigFlags& temperature_resolution, const InitConfigFlags& pressure_oversampling, const InitConfigFlags& humidity_oversampling, const InitConfigFlags& mode)
{
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low
    HAL_Delay(5);
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high

    int i;
    for( i = 0 ; i < 3 ; i++ )
        t[i]=read_16_combine(DIG_T[i]);

    for (i = 0 ; i < 9 ; i++)
        p[i] = read_16_combine(DIG_P[i]);

    h[0] = read_8(DIG_H[0]);
    h[1] = read_16_combine(DIG_H[1]);
    h[2] = read_8(DIG_H[2]);
    h[3] = ((read_8(DIG_H[3]) << 4 ) | (read_8(DIG_H[3]+1) & 0xF) );
    h[4] = ((read_8(DIG_H[4]+1) << 4 ) | (read_8(DIG_H[4]) >> 4) );
    h[5] = (int8_t)read_8(DIG_H[5]);

    const uint8_t HumReg =( read_8(static_cast<uint8_t>(Command::BME280_HUM_CONTROL)) & 0xF8 ) | (static_cast<uint8_t>(humidity_oversampling));

    write_8(static_cast<uint8_t>(Command::BME280_HUM_CONTROL), HumReg);
    write_8(0xF4, ((static_cast<uint8_t>(temperature_resolution)<<5) | ((static_cast<uint8_t>(pressure_oversampling)<<2) | (static_cast<uint8_t>(mode)))));
}

void Sensor::sensor_set_config(const InitConfigFlags& standby_time, const InitConfigFlags& filter)
{
    write_8(static_cast<uint8_t>(Command::BME280_CONFIG) , (uint8_t) (((static_cast<uint8_t>(standby_time) & 0x7) << 5) | ((static_cast<uint8_t>(filter) & 0x7) << 2)) & 0xFC);
}

void Sensor::read_all(float& temperature, float& pressure, float& humidity)
{
    read_temperature(temperature);

    read_pressure(pressure);

    read_humidity(humidity);

}

void Sensor::read_temperature(float& temperature)
{
    //Reading
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low
    int32_t adc_T = read24(static_cast<uint8_t>(Command::BME280_TEMPDATA));
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high to end reading

    temperature = convert_data_temperature(adc_T);
}
float Sensor::convert_data_temperature(int32_t adc_T)
{
    adc_T >>= 4;

    const int32_t var1  = ((((adc_T>>3) - ((int32_t)t[0] << 1))) * ((int32_t)t[1])) >> 11;

    const int32_t var2  = (((((adc_T>>4) - ((int32_t)t[0])) * ((adc_T >> 4) - ((int32_t)t[0]))) >> 12) * ((int32_t)t[2])) >> 14;

    t_fine = var1 + var2;

    float T  = (t_fine * 5 + 128) >> 8;

    return T/100;
}

void Sensor::read_pressure(float& pressure){

    //Reading
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low to start reading
    int32_t adc_P = read24(static_cast<uint8_t>(Command::BME280_PRESSUREDATA));
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin high to end reading

    pressure = convert_data_pressure(adc_P);


}
int32_t Sensor::convert_data_pressure(int32_t adc_P){

    int64_t var1, var2, P;
    adc_P >>= 4;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)p[5];
    var2 = var2 + ((var1*(int64_t)p[4]) << 17);
    var2 = var2 + (((int64_t)p[3]) << 35);
    var1 = ((var1 * var1 * (int64_t)p[2]) >> 8) + ((var1 * (int64_t)p[1]) << 12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)p[0]) >> 33;

    if (var1 == 0) {
        return 0;
    }
    P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)p[8]) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((int64_t)p[7]) * P) >> 19;

    P = ((P + var1 + var2) >> 8) + (((int64_t)p[6]) << 4);
    return (int32_t)P / 256;
}

void Sensor::read_humidity(float& humidity){

    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);  // pull the pin low to start reading
    int32_t adc_H = read_16(static_cast<uint8_t>(Command::BME280_HUMIDDATA));
    HAL_GPIO_WritePin (GPIOC, GPIO_PIN_3, GPIO_PIN_SET);  // pull the pin low to start reading

    humidity = convert_data_humidity(adc_H);
}
int32_t Sensor::convert_data_humidity(int32_t adc_H) {

    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)h[3]) << 20) -
                    (((int32_t)h[4]) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)h[5])) >> 10) *
                      (((v_x1_u32r * ((int32_t)h[2])) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)h[1]) + 8192) >> 14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)h[0])) >> 4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;

    return (v_x1_u32r>>12)/ 1024.0;
}

void Sensor::write_8( const uint8_t address, const uint8_t data)
{
    uint8_t tmp[2];
	tmp[0] = address & ~(1<<7);
	tmp[1] = data;
    transmit_receive(2,tmp);

}

uint16_t Sensor::read_16_combine( const uint8_t addr)
{
    uint16_t tmp = read_16(addr);
    return (tmp >> 8) | (tmp << 8);
}
uint16_t Sensor::read_16(const uint8_t addr)
{
    uint8_t tmp[3];
    tmp[0] = addr | (1 << 7);
    transmit_receive(3,tmp);
    return (tmp[1] << 8) | tmp[2];
}

uint8_t Sensor::read_8(const uint8_t addr)
{
    uint8_t tmp[2];
    tmp[0] = addr|(1<<7);
    transmit_receive(2, tmp);
    return tmp[1];
}

uint32_t Sensor::read24(const uint8_t addr)
{
    uint8_t tmp[4];
    tmp[0] = addr | (1<<7);
    transmit_receive(4,tmp);
    return ((tmp[1] << 16) | tmp[2] << 8 | tmp[3]);
}

void Sensor::transmit_receive(const size_t size, uint8_t *tmp)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive(spi_h, tmp, tmp, size, 10);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
}

void Sensor::measure_task_function(void *args)
{
    Sensor& sensor = obc.peripherals.sensor;
    Lcd& lcd = obc.peripherals.lcd;

    float temperature, pressure, humidity;
    char buf[16];

    auto print_lambda = [&buf](const char* text, float value){
      sprintf(buf, text, value);
      lcd.print_line(3, buf);
      os::Task::delay(1000);
    };

    while(true)
    {
        sensor.read_all(temperature,pressure,humidity);
        print_lambda("Temp: %.2lf C", temperature);
        print_lambda("Press: %.2lf hPa", pressure/100);
        print_lambda("Hum: %.2lf %%RH",humidity);
    }
}

const os::Task &Sensor::getMeasureTask() const
{
    return measure_task;
}
