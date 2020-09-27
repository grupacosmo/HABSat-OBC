//
// Created by Wiktor Wieclaw on 8/25/20.
//

#ifndef RCC_SYS_IDLE_TASKS_H
#define RCC_SYS_IDLE_TASKS_H

#include "peripherals.h"
#include "os_task.h"

extern global::Peripherals g_peripherals;

namespace task_code
{
    namespace idle
    {
        void led(void *args);

        void lcd(void *args);
    }
}

#endif //RCC_SYS_IDLE_TASKS_H
