//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_HANDLES_H
#define RCC_SYS_HANDLES_H

//#include "stm32f4xx.h"

#include <stm32f4xx.h>

namespace hw
{

using I2CHandle = I2C_HandleTypeDef;
using SPIHandle = SPI_HandleTypeDef;
using UARTHandle = UART_HandleTypeDef;
using SDIOHandle = SD_HandleTypeDef;

struct Handles
{
    static I2CHandle *const i2c3;
    static SPIHandle *const spi2;
    static UARTHandle *const uart1;
    static SDIOHandle *const sdio;
};

}

#endif//RCC_SYS_HANDLES_H