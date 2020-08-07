//
// Created by vixu on 8/7/20.
//

#include "led.h"

void Led::on()
{
    HAL_GPIO_WritePin(type, pin, GPIO_PIN_SET);
}
void Led::off()
{
    HAL_GPIO_WritePin(type, pin, GPIO_PIN_RESET);
}
void Led::toggle()
{
    HAL_GPIO_TogglePin(type, pin);
}



