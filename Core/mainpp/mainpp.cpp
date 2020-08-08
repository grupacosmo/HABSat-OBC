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
    Lcd lcd(Lcd::Interface::_4_BIT, 4, 20, &hi2c1, lcd_slave_address);

    int i = 0;
    while(true)
    {
        led.toggle();
        HAL_Delay(500);
        i = ++i % 4;
        lcd.print_line(i, "dobra gitara");
        HAL_Delay(500);
        lcd.clear();
    }
}
