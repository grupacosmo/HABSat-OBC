//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "interrupt_tasks.h"

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
