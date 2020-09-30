//
// Created by Wiktor Wieclaw on 8/6/20.
//


#include "mainpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "sensor.h"


constexpr uint8_t lcd_slave_address = 0x4E;
extern I2C_HandleTypeDef hi2c1;
extern SPI_HandleTypeDef hspi2;


void mainpp()
{

    float temperature, pressure;
    char buf[10], buf1[10];

    Sensor sensor;
    Led led;
    Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    uint32_t delay = 500;
    HAL_Delay(2 * delay);


    auto lambda = [lcd, delay](const uint16_t line, const std::string& s){

        lcd.print_line(line, s);
        HAL_Delay(delay);
    };
    sensor.init(&hspi2, 1, 1, 3, 3);
    sensor.sensor_set_config(6, 0);
    HAL_Delay(3000);

    while(true)
    {

        /*Temperature*/

        sensor.read_temperature(&temperature);
        sprintf(buf,"Temp: %.2lf C",temperature);
        lcd.print_line(0,buf);

        /*Pressure*/

        sensor.read_pressure(&pressure);
        sprintf(buf1,"Press: %.2lf hPa",pressure/100);
        lcd.print_line(1,buf1);

        lcd.clear();

    }
}
