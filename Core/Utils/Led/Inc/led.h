//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef FREERTOS_INTRO_LED_H
#define FREERTOS_INTRO_LED_H

#include "stm32f4xx_hal.h"
#include "os_task.h"

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
    bool is_on() const;

    /**
     * Getter for blink task object.
     * @return
     */
    const os::Task &get_blink_task() const;

private:
    static void blink_task_function(void *args);

private:
    const os::Task blink_task{"blink", 128, os::Task::Priority::IDLE, blink_task_function};

    GPIO_TypeDef *const TYPE = GPIOA;
    const uint16_t PIN = GPIO_PIN_5;
};


#endif //FREERTOS_INTRO_LED_H
