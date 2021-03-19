//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef HABSAT_OBC_HANDLES_H
#define HABSAT_OBC_HANDLES_H

#include <stm32f4xx.h>

extern I2C_HandleTypeDef hi2c3;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern SD_HandleTypeDef hsd;

#endif  // HABSAT_OBC_HANDLES_H