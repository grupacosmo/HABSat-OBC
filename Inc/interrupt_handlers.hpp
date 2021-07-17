//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef HABSAT_OBC_INTERRUPTHANDLERS_H
#define HABSAT_OBC_INTERRUPTHANDLERS_H

#include <stdint.h>
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

EXTERN_C void USER_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);

EXTERN_C void GPS_IRQHandler(UART_HandleTypeDef *huart);

#endif  // HABSAT_OBC_INTERRUPTHANDLERS_H
