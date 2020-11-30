//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "Blink.h"
#include "obc.h"

namespace services {

os::Task Blink::inputTask_{"input", 128, os::Priority::Interrupt, inputTaskFunction};
os::Task Blink::blinkTask_{"blink", 128, os::Priority::Idle, blinkTaskFunction};

Blink::Blink(hw::Led *led)
    : led_(led)
{

}

void Blink::init()
{
    blinkTask_.addToScheduler(static_cast<void*>(led_));
    inputTask_.addToScheduler();
}

void Blink::inputInterruptHandler()
{
    getInputTask().resumeFromISR();
}

os::Task &Blink::getBlinkTask()
{
    return blinkTask_;
}

os::Task &Blink::getInputTask()
{
    return inputTask_;
}

void Blink::blinkTaskFunction(void *args)
{
    auto led = static_cast<hw::Led*>(args);
    while (true)
    {
        led->toggle();
        os::Task::delay(1000);
    }
}

void Blink::inputTaskFunction(void *args)
{
    (void)args;
    while (true)
    {
        os::Task::suspendItself();
        if (Blink::getBlinkTask().getState() != os::TaskState::Suspended)
            Blink::getBlinkTask().suspend();
        else
            Blink::getBlinkTask().resume();
        os::Task::delay(1000);
    }
}

}