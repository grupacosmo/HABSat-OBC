//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "idle_tasks.h"

void led_task_code(void *args)
{
    while(true)
    {
        led.toggle();
        os::Task::delay(1000);
    }
}

void lcd_task_code(void *args)
{
    const int delay_ms = 1111;
    while(true)
    {
        lcd.print_line(0, "display");
        os::Task::delay(delay_ms);
        lcd.print_line(0, "test");
        os::Task::delay(delay_ms);
    }
}

void rtc_task_code(void *args)
{
    char time[20] = {};
    char date[20] = {};
    while(true) {
        *time = {};
        *date = {};
        rtc.get_time_date();
        rtc.time_info(time);
        rtc.date_info(date);
        lcd.print_line(1, date);
        lcd.print_line(2, time);
        os::Task::delay(257);
    }
}
