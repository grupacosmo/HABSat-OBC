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
extern SPI_HandleTypeDef hspi1;


void mainpp()
{

    float temperature, huminidity;
    char buf[10], buf1[10];
    sprintf(buf1,"%lf",temperature);
    int32_t pressure;

    Sensor sensor;
    Led led;
    Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    uint32_t delay = 500;
    HAL_Delay(2 * delay);


    auto lambda = [lcd, delay](const uint16_t line, const std::string& s){

        lcd.print_line(line, s);
        HAL_Delay(delay);
    };
    sensor.init(&hspi1, 1, 1, 3, 3);
    sensor.sensor_set_config(6, 0);

    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // pull the pin low

    sensor.read_temperature_a(&temperature);
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  // pull the pin high




    while(true)
    {
        lcd.print_line(0,"Temperature:");
        lcd.print_line(1,buf1);
        sprintf(buf,"%lf",temperature);
        lcd.print_line(2,"Temperature: ");
        lcd.print_line(3,buf);

    }
}
