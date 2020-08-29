//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "led.h"

void Led::on() const
{
    HAL_GPIO_WritePin(TYPE, PIN, GPIO_PIN_SET);
}
void Led::off() const
{
    HAL_GPIO_WritePin(TYPE, PIN, GPIO_PIN_RESET);
}
void Led::toggle() const
{
    HAL_GPIO_TogglePin(TYPE, PIN);
}

bool Led::isOn() const
{
    return HAL_GPIO_ReadPin(TYPE, PIN) == GPIO_PIN_SET;
}



