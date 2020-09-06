//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_INTERRUPT_TASKS_H
#define RCC_SYS_INTERRUPT_TASKS_H

#include "idle_tasks.h"
#include "os_queue.h"
#include "stm32f4xx.h"
#include "wifi.h"

/* tasks */
extern const os::Task led_task;
extern const os::Task button_interrupt_task;

/* handles */
extern DMA_HandleTypeDef hdma_usart1_rx;

/* modules */
extern WiFi wifi;
extern Lcd lcd;

os::Queue<int, 25> uart_notification_queue;

void button_interrupt_task_code(void *args);
void uart_receive_interrupt_task_code(void *args);


#endif //RCC_SYS_INTERRUPT_TASKS_H
