//
// Created by Wiktor Wieclaw on 10/3/20.
//

#ifndef RCC_SYS_BUTTON_TASKS_H
#define RCC_SYS_BUTTON_TASKS_H

#include "led_tasks.h"
#include "os_task.h"
#include "peripherals.h"

static void button_code(void *args)
{
    while (true)
    {
        os::Task::suspend_itself();
        peripherals.lcd.print_line(3, "click");
        if (led_task.get_state() != os::Task::State::eSuspended)
            led_task.suspend();
        else
            led_task.resume();
        os::Task::delay(1000);
        peripherals.lcd.print_line(3, "");
    }
}

#endif//RCC_SYS_BUTTON_TASKS_H
