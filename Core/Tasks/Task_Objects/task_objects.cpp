//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "task_objects.h"

namespace global
{
    void Tasks::initialize()
    {
        led_task.add_to_scheduler();
        lcd_task.add_to_scheduler();
        button_interrupt_task.add_to_scheduler();
    }
}
