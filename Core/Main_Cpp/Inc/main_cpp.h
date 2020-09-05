//
// Created by Wiktor Wieclaw on 8/6/20.
//

#ifndef FREERTOS_INTRO_MAIN_CPP_H
#define FREERTOS_INTRO_MAIN_CPP_H

#ifdef __cplusplus
extern "C"
#endif
void main_cpp();

// TODO:
#include "stm32f4xx.h"
#ifdef __cplusplus
extern "C"
#endif
void USER_UART_IRQHandler(UART_HandleTypeDef *huart);
#endif //FREERTOS_INTRO_MAIN_CPP_H
