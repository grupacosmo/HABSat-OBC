//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef FREERTOS_INTRO_LED_H
#define FREERTOS_INTRO_LED_H

#include "stm32f4xx_hal.h"

class Led{
    GPIO_TypeDef *type = GPIOA;
    uint16_t pin = GPIO_PIN_5;
public:
    void on() const;
    void off() const;
    void toggle() const;
};


#endif //FREERTOS_INTRO_LED_H
