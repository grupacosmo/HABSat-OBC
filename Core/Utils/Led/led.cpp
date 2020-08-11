//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "led.h"

void Led::on() const
{
    HAL_GPIO_WritePin(type, pin, GPIO_PIN_SET);
}
void Led::off() const
{
    HAL_GPIO_WritePin(type, pin, GPIO_PIN_RESET);
}
void Led::toggle() const
{
    HAL_GPIO_TogglePin(type, pin);
}



