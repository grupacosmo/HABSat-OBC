//
// Created by Wiktor Wieclaw on 10/3/20.
//

#ifndef RCC_SYS_UTILS_H
#define RCC_SYS_UTILS_H

#include "button.h"
#include "led.h"

/**
 * Holds objects that cannot be categorized as peripherals.
 */
struct Utils
{
    Button button;
    Led led;

    void init();
};

#endif//RCC_SYS_UTILS_H
