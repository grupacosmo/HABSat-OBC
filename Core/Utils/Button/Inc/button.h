//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_BUTTON_H
#define RCC_SYS_BUTTON_H

#include "stm32f4xx.h"
#include "os_task.h"
#include "button_tasks.h"

class Button
{
public:
    const os::Task task{"button", 128, os::Task::Priority::INTERRUPT, button_code};
private:
    GPIO_TypeDef *const TYPE = GPIOC;
    const uint16_t PIN = GPIO_PIN_13;
public:
    bool is_pressed() const;
};

#endif //RCC_SYS_BUTTON_H
