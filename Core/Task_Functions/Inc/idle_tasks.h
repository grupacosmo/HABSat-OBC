//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_IDLE_TASKS_H
#define RCC_SYS_IDLE_TASKS_H

#include "freertoswrapper.h"
#include "led.h"
#include "lcd.h"

extern const Led led;
extern Lcd lcd;

void led_task_code(void *args)
{
    while(true)
    {
        led.toggle();
        os::Task::delay(1000);
    }
}

void lcd_task_code(void *args)
{
    const int delay_ms = 1111;
    while(true)
    {
        lcd.print_line(0, "display");
        os::Task::delay(delay_ms);
        lcd.print_line(0, "test");
        os::Task::delay(delay_ms);
    }
}

#endif //RCC_SYS_IDLE_TASKS_H
