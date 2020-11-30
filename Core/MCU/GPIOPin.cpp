//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "GPIOPin.h"

namespace hw {

GPIOPin::GPIOPin(GPIO_TypeDef *pinType, uint16_t pinNumber)
    : pinType_(pinType), pinNumber_(pinNumber)
{

}

void GPIOPin::set()
{
    HAL_GPIO_WritePin(pinType_, pinNumber_, GPIO_PIN_SET);
}

void GPIOPin::reset()
{
    HAL_GPIO_WritePin(pinType_, pinNumber_, GPIO_PIN_RESET);
}

void GPIOPin::toggle()
{
    HAL_GPIO_TogglePin(pinType_, pinNumber_);
}

GPIOState GPIOPin::read() const
{
    return static_cast<GPIOState>(HAL_GPIO_ReadPin(pinType_, pinNumber_));
}

}