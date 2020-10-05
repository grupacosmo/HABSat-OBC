//
// Created by Wiktor Wieclaw on 10/1/20.
//

#ifndef RCC_SYS_LCD_TASKS_H
#define RCC_SYS_LCD_TASKS_H

#include "os_task.h"
#include "peripherals.h"

void lcd_task_code(void *args)
{
    const int delay_ms = 1111;
    while (true)
    {
        peripherals.lcd.print_line(0, "display");
        os::Task::delay(delay_ms);
        peripherals.lcd.print_line(0, "test");
        os::Task::delay(delay_ms);
    }
}
const os::Task lcd_task{"lcd", 128, os::Task::Priority::IDLE, lcd_task_code};

#endif//RCC_SYS_LCD_TASKS_H
