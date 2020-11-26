//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef FREERTOS_INTRO_LED_H
#define FREERTOS_INTRO_LED_H

#include "os_task.h"
#include "stm32f4xx_hal.h"

namespace hw {

class Led
{
public:
    /**
     * Adds task into the scheduler.
     */
    void init();

    /**
     * Turns the LED on.
     */
    void on() const;

    /**
     * Turns the LED off.
     */
    void off() const;

    /**
     * Toggles the LED.
     */
    void toggle() const;

    /**
     * Checks whether the LED is on.
     * @return
     */
    bool isOn() const;

    /**
     * Getter for blink task object.
     * @return
     */
    const os::Task &getBlinkTask() const;

private:
    static void blinkTaskFunction(void *args);

private:
    const os::Task blinkTask_{"blink", 128, os::Priority::idle, blinkTaskFunction};

    GPIO_TypeDef *const type_ = GPIOA;
    const uint16_t pin_ = GPIO_PIN_5;
};

}

#endif //FREERTOS_INTRO_LED_H
