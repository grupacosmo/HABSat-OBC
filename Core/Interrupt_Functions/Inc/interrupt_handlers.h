//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_INTERRUPT_HANDLERS_H
#define RCC_SYS_INTERRUPT_HANDLERS_H

#include "interrupt_tasks.h"

extern UART_HandleTypeDef huart1;

#ifdef __cplusplus
extern "C"
#endif
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);

#ifdef __cplusplus
extern "C"
#endif
void USER_UART_DMA_IRQHandler(DMA_HandleTypeDef *hdma_usart_rx);


#endif//RCC_SYS_INTERRUPT_HANDLERS_H
