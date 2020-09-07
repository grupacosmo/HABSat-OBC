//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_TASK_OBJECTS_H
#define RCC_SYS_TASK_OBJECTS_H
#include "idle_tasks.h"
#include "interrupt_tasks.h"
#include "os_task.h"

namespace global
{
    struct Tasks
    {
        /* idle tasks */
        const os::Task led_task{"lcd_task", 128, os::Task::Priority::IDLE, led_task_code};
        const os::Task lcd_task{"lcd_task", 128, os::Task::Priority::IDLE, lcd_task_code};

        /* interrupt tasks */
        const os::Task button_interrupt_task{"button_task", 128, os::Task::Priority::INTERRUPT, button_interrupt_task_code};

        void initialize();
    };
}

#endif//RCC_SYS_TASK_OBJECTS_H
