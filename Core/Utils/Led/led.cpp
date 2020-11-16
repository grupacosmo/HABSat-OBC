//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "led.h"
#include "../../OBC/obc.h"
namespace hw {

void Led::on() const
{
    HAL_GPIO_WritePin(TYPE, PIN, GPIO_PIN_SET);
}
void Led::off() const
{
    HAL_GPIO_WritePin(TYPE, PIN, GPIO_PIN_RESET);
}
void Led::toggle() const
{
    HAL_GPIO_TogglePin(TYPE, PIN);
}

bool Led::is_on() const
{
    return HAL_GPIO_ReadPin(TYPE, PIN) == GPIO_PIN_SET;
}

void Led::blink_task_function(void *args)
{
    (void)args;
    while (true)
    {
        obc.hardware.led.toggle();
        os::Task::delay(1000);
    }
}
void Led::init()
{
    blink_task.addToScheduler();
}
const os::Task &Led::get_blink_task() const
{
    return blink_task;
}

}
