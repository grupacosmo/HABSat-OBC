//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "obc.h"

extern I2C_HandleTypeDef hi2c3;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart1;
extern SD_HandleTypeDef hsd;

namespace hw
{
I2CHandle *const Handles::i2c3 = &hi2c3;
SPIHandle *const Handles::spi2 = &hspi2;
UARTHandle *const Handles::uart1 = &huart1;
SDIOHandle *const Handles::sdio = &hsd;
}

void Obc::init()
{
    hardware.init();
}
