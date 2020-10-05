//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef FREERTOS_INTRO_LED_H
#define FREERTOS_INTRO_LED_H

#include "stm32f4xx_hal.h"

class Led
{
private:
    GPIO_TypeDef *const TYPE = GPIOA;
    const uint16_t PIN = GPIO_PIN_5;
public:
    void on() const;
    void off() const;
    void toggle() const;
    bool isOn() const;
};


#endif //FREERTOS_INTRO_LED_H
