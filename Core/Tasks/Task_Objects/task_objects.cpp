//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "task_objects.h"

namespace global
{
    void Tasks::initialize()
    {
        led.add_to_scheduler();
        lcd.add_to_scheduler();
        button_interrupt.add_to_scheduler();
    }
}
