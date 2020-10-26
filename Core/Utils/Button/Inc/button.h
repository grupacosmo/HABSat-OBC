//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_BUTTON_H
#define RCC_SYS_BUTTON_H

#include "stm32f4xx.h"
#include "os_task.h"

class Button
{
public:

    /**
     * Add task into the scheduler
     */
    void init();

    /**
     * Checks whether the button is pressed
     * @return
     */
    bool is_pressed() const;

    /**
     * Getter for the task object
     * @return
     */
    const os::Task &get_input_task() const;

private:
    static void input_task_function(void *args);

private:
    const os::Task input_task{"input", 128, os::Task::Priority::INTERRUPT, input_task_function};

    GPIO_TypeDef *const TYPE = GPIOC;
    const uint16_t PIN = GPIO_PIN_13;
};

#endif //RCC_SYS_BUTTON_H
