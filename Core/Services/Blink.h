//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_BLINK_H
#define RCC_SYS_BLINK_H

#include "os_task.h"
#include "led.h"
#include "button.h"

namespace services
{

class Blink
{
public:
    Blink(hw::Led *led);
    void init();
public:
    static void inputInterruptHandler();
private:
    static os::Task& getBlinkTask();
    static os::Task& getInputTask();
private:
    static void inputTaskFunction(void *args);
    static void blinkTaskFunction(void *args);
private:
    static os::Task inputTask_;
    static os::Task blinkTask_;
private:
    hw::Led *led_;
};

}
#endif//RCC_SYS_BLINK_H
