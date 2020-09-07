//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_INTERRUPT_TASKS_H
#define RCC_SYS_INTERRUPT_TASKS_H

#include "os_queue.h"
#include "stm32f4xx.h"
#include "peripherals.h"

/* modules */
extern global::Peripherals g_peripherals;

void button_interrupt_task_code(void *args);
void uart_receive_interrupt_task_code(void *args);


#endif //RCC_SYS_INTERRUPT_TASKS_H
