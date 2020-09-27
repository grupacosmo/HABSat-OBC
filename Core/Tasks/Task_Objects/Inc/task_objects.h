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
        const os::Task led{"lcd", 128, os::Task::Priority::IDLE, task_code::idle::led};
        const os::Task lcd{"lcd", 128, os::Task::Priority::IDLE, task_code::idle::lcd};

        /* interrupt tasks */
        const os::Task button_interrupt{"button", 128, os::Task::Priority::INTERRUPT, task_code::interrupt::button};

        void initialize();
    };
}

#endif//RCC_SYS_TASK_OBJECTS_H
