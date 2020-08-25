//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_INTERRUPT_TASKS_H
#define RCC_SYS_INTERRUPT_TASKS_H

#include "stm32f4xx.h"
#include "idle_tasks.h"

extern const os::Task led_task;
extern const os::Task button_interrupt_task;

void button_interrupt_task_code(void *args)
{
    while(true)
    {
        os::Task::suspend_itself();
        lcd.print_line(3, "click");
        if(led_task.get_state() != os::Task::State::eSuspended)
            led_task.suspend();
        else
            led_task.resume();
        os::Task::delay(1000);
        lcd.print_line(3, "");
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_13:
            button_interrupt_task.resume_from_ISR();
            break;
        default:
            break;
    }
}

#endif //RCC_SYS_INTERRUPT_TASKS_H
