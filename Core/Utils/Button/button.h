//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_BUTTON_H
#define RCC_SYS_BUTTON_H

#include "os_task.h"
#include "stm32f4xx.h"

namespace hw {

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
    const os::Task input_task{"input", 128, os::Priority::INTERRUPT, input_task_function};

    GPIO_TypeDef *const TYPE = GPIOC;
    static constexpr uint16_t PIN = GPIO_PIN_13;
};

}

#endif //RCC_SYS_BUTTON_H
