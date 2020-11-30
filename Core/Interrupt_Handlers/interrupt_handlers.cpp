//
// Created by Wiktor Wieclaw on 9/5/20.
//

#include "interrupt_handlers.h"
#include "obc.h"
#include "os_queue.h"
#include "handles.h"

void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);
        switch(GPIO_Pin)
        {
            case GPIO_PIN_13:
                services::Blink::inputInterruptHandler();
                break;
            default:
                break;
        }
    }
}
