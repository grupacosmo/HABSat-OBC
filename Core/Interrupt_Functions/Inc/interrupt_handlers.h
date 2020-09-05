//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_INTERRUPT_HANDLERS_H
#define RCC_SYS_INTERRUPT_HANDLERS_H

#include "interrupt_tasks.h"

extern DMA_HandleTypeDef hdma_usart1_rx;
std::array<char, 255> receive_buffer;

void USER_UART_IRQHandler(UART_HandleTypeDef *huart);


#endif//RCC_SYS_INTERRUPT_HANDLERS_H
