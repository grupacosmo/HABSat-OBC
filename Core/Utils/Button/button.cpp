//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include <button.h>
#include "obc.h"

bool Button::is_pressed() const
{
    return HAL_GPIO_ReadPin(TYPE, PIN) == GPIO_PIN_RESET;
}

void Button::input_task_function(void *args)
{
    (void)args;
    Lcd &lcd = obc.peripherals.lcd;
    const os::Task &led_blink_task = obc.utils.led.get_blink_task();
    while (true)
    {
        os::Task::suspend_itself();
        lcd.print_line(3, "click");
        if (led_blink_task.get_state() != os::Task::State::eSuspended)
            led_blink_task.suspend();
        else
            led_blink_task.resume();
        os::Task::delay(1000);
        lcd.print_line(3, "");
    }
}
void Button::init()
{
    input_task.addToScheduler();
}
const os::Task &Button::get_input_task() const
{
    return input_task;
}
