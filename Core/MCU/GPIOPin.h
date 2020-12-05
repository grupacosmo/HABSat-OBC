//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_GPIOPIN_H
#define RCC_SYS_GPIOPIN_H

#include "stm32f4xx.h"
#include "Noncopyable.h"

namespace hw {

enum GPIOState
{
    Reset = GPIO_PIN_RESET,
    Set   = GPIO_PIN_SET
};

class GPIOPin : public Noncopyable
{
public:
    GPIOPin(GPIO_TypeDef* pinType, uint16_t pinNumber);
    void set();
    void reset();
    void toggle();
    GPIOState read() const;
private:
    GPIO_TypeDef *const pinType_;
    const uint16_t pinNumber_;
};

}

#endif//RCC_SYS_GPIOPIN_H
