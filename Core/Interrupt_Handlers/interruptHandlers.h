//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_INTERRUPTHANDLERS_H
#define RCC_SYS_INTERRUPTHANDLERS_H

#include "stm32f4xx.h"

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);

#endif  // RCC_SYS_INTERRUPTHANDLERS_H
