//
// Created by vixu on 8/6/20.
//

#include "mainpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"

constexpr uint8_t lcd_slave_address = 0x4E;
extern I2C_HandleTypeDef hi2c1;

void mainpp()
{
    Led led;
    Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    uint32_t delay = 500;
    HAL_Delay(2 * delay);

    auto lambda = [lcd, delay](const uint16_t line, const std::string& s){

        lcd.print_line(line, s);
        HAL_Delay(delay);
    };
    while(true)
    {
        lambda(0, "stm32");
        lambda(1, "liquid");
        lambda(2, "crystal");
        lambda(3, "display");
        lcd.clear();
        lambda(0, "i2c");
        lambda(1, "c++17");
        lambda(2, "print_line()");
        lambda(3, "test");
        lcd.clear();
    }
}
