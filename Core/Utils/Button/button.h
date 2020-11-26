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
    bool isPressed() const;

    /**
     * Getter for the task object
     * @return
     */
    const os::Task &getInputTask() const;

private:
    static void inputTaskFunction(void *args);

private:
    const os::Task inputTask_{"input", 128, os::Priority::Interrupt, inputTaskFunction};

    GPIO_TypeDef *const type_ = GPIOC;
    static constexpr uint16_t pin_ = GPIO_PIN_13;
};

}

#endif //RCC_SYS_BUTTON_H
