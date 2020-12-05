//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef HABSAT_OBC_INTERRUPTHANDLERS_H
#define HABSAT_OBC_INTERRUPTHANDLERS_H

#include "stm32f4xx.h"

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);

#endif  // HABSAT_OBC_INTERRUPTHANDLERS_H
