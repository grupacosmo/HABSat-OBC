//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "idle_tasks.h"

namespace task_code
{
    namespace idle
    {
        void led(void *args)
        {
            while (true)
            {
                g_peripherals.led.toggle();
                os::Task::delay(1000);
            }
        }

        void lcd(void *args)
        {
            const int delay_ms = 1111;
            while (true)
            {
                g_peripherals.lcd.print_line(0, "display");
                os::Task::delay(delay_ms);
                g_peripherals.lcd.print_line(0, "test");
                os::Task::delay(delay_ms);
            }
        }
    }
}