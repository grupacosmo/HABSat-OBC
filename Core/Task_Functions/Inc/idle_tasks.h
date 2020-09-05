//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_IDLE_TASKS_H
#define RCC_SYS_IDLE_TASKS_H

#include "lcd.h"
#include "led.h"
#include "os_task.h"

extern const Led led;
extern Lcd lcd;

void led_task_code(void *args);

void lcd_task_code(void *args);

#endif //RCC_SYS_IDLE_TASKS_H
