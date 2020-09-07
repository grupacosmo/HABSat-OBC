//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_INTERRUPT_HANDLERS_H
#define RCC_SYS_INTERRUPT_HANDLERS_H

#include "interrupt_tasks.h"
#include "task_objects.h"
#include "handles.h"

extern "C" void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
extern "C" void USER_UART_DMA_IRQHandler(DMA_HandleTypeDef *hdma_usart_rx);
extern "C" void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);

#endif//RCC_SYS_INTERRUPT_HANDLERS_H
