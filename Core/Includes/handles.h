//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_HANDLES_H
#define RCC_SYS_HANDLES_H

#include "stm32f4xx.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;

#endif//RCC_SYS_HANDLES_H