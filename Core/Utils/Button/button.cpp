//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include "button.h"
#include "obc.h"

namespace hw {

bool Button::isPressed() const
{
    return HAL_GPIO_ReadPin(type_, pin_) == GPIO_PIN_RESET;
}

void Button::inputTaskFunction(void *args)
{
    (void)args;
    hw::Lcd &lcd = obc().hardware.lcd;
    const os::Task &led_blink_task = obc().hardware.led.getBlinkTask();
    while (true)
    {
        os::Task::suspendItself();
        lcd.print_line(3, "click");
        if (led_blink_task.getState() != os::Task::State::eSuspended)
            led_blink_task.suspend();
        else
            led_blink_task.resume();
        os::Task::delay(1000);
        lcd.print_line(3, "");
    }
}

void Button::init()
{
    inputTask_.addToScheduler();
}
const os::Task &Button::getInputTask() const
{
    return inputTask_;
}

}