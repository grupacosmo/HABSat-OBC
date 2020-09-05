//
// Created by Wiktor Wieclaw on 8/29/20.
//

#ifndef RCC_SYS_INTERRUPT_CALLBACKS_H
#define RCC_SYS_INTERRUPT_CALLBACKS_H

#include "interrupt_tasks.h"

/*
 * Functions need to be implemented in the header file
 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case GPIO_PIN_13:
            button_interrupt_task.resume_from_ISR();
            break;
        default:
            break;
    }
}

#endif //RCC_SYS_INTERRUPT_CALLBACKS_H
