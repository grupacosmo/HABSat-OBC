//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "led.h"
#include "../../OBC/obc.h"
namespace hw {

void Led::on() const
{
    HAL_GPIO_WritePin(type_, pin_, GPIO_PIN_SET);
}
void Led::off() const
{
    HAL_GPIO_WritePin(type_, pin_, GPIO_PIN_RESET);
}
void Led::toggle() const
{
    HAL_GPIO_TogglePin(type_, pin_);
}

bool Led::isOn() const
{
    return HAL_GPIO_ReadPin(type_, pin_) == GPIO_PIN_SET;
}

void Led::blinkTaskFunction(void *args)
{
    (void)args;
    while (true)
    {
        obc().hardware.led.toggle();
        os::Task::delay(1000);
    }
}
void Led::init()
{
    blinkTask_.addToScheduler();
}
const os::Task &Led::getBlinkTask() const
{
    return blinkTask_;
}

}
