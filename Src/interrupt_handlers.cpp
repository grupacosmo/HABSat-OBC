//
// Created by Wiktor Wieclaw on 9/5/20.
//

#include "interrupt_handlers.hpp"

#include "obc.hpp"

void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin) {
    if (__HAL_GPIO_EXTI_GET_IT(GPIO_Pin) != RESET) {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_Pin);

        switch (GPIO_Pin) {
            case GPIO_PIN_13:
                habsat::blink::inputInterruptHandler();
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

void GPS_IRQHandler(UART_HandleTypeDef *huart){
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) == SET) {
        __HAL_UART_CLEAR_IDLEFLAG(huart); // clear idle interrupt sign
        habsat::getObc().gps.callback();
    }
}



