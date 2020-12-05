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

namespace hw {

using I2CHandle  = I2C_HandleTypeDef;
using SPIHandle  = SPI_HandleTypeDef;
using UARTHandle = UART_HandleTypeDef;
using SDIOHandle = SD_HandleTypeDef;

struct Handles {
  constexpr static I2CHandle* const i2c3   = &hi2c3;
  constexpr static SPIHandle* const spi2   = &hspi2;
  constexpr static UARTHandle* const uart1 = &huart1;
  constexpr static SDIOHandle* const sdio  = &hsd;
};

}  // namespace hw

#endif  // HABSAT_OBC_HANDLES_H