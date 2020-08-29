//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_INTERRUPT_TASKS_H
#define RCC_SYS_INTERRUPT_TASKS_H

#include "stm32f4xx.h"
#include "idle_tasks.h"

extern Lcd lcd;
extern const os::Task led_task;
extern const os::Task button_interrupt_task;

void button_interrupt_task_code(void *args);


#endif //RCC_SYS_INTERRUPT_TASKS_H
