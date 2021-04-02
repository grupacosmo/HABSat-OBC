//
// Created by Wiktor Wieclaw on 9/5/20.
//

#include "interruptHandlers.h"

#include "Obc.h"

void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin) {
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);

        switch (GPIO_Pin) {
            case GPIO_PIN_13:
                blink::inputInterruptHandler();
                break;
            case GPIO_PIN_14:
                asm("nop");
                // ^ this surprises warning about changing switch to if/else block
                // if you are adding another handle then delete or change this
                break;
            default:
                break;
        }
    }
}
