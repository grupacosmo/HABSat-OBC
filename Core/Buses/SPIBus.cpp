//
// Created by Wiktor Wieclaw on 11/16/20.
//

#include "SPIBus.h"

namespace hw
{

SPIBus::SPIBus(SPIHandle *handle) : Bus(handle)
{

}
void SPIBus::clearCS(const ChipSelect& cs) const
{
    HAL_GPIO_WritePin(cs.gpioType, cs.pinNumber, GPIO_PIN_RESET);
}
void SPIBus::setCS(const ChipSelect& cs) const
{
    HAL_GPIO_WritePin(cs.gpioType, cs.pinNumber, GPIO_PIN_SET);
}
BusResult SPIBus::transmit(uint8_t *data, uint16_t size, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_SPI_Transmit(handle, data, size, timeout));
}
BusResult SPIBus::transmitAndReceive(uint8_t *txData, uint8_t *rxData, uint16_t size, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_SPI_TransmitReceive(handle, txData, rxData, size, timeout));
}

}