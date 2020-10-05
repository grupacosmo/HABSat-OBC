//
// Created by Wiktor Wieclaw on 10/3/20.
//

#ifndef RCC_SYS_LED_TASKS_H
#define RCC_SYS_LED_TASKS_H

#include "os_task.h"
#include "peripherals.h"

static void led_code(void *args)
{
    while (true)
    {
        peripherals.led.toggle();
        os::Task::delay(1000);
    }
}

const os::Task led_task{"lcd", 128, os::Task::Priority::IDLE, led_code};

#endif//RCC_SYS_LED_TASKS_H
