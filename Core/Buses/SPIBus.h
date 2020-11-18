//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_SPIBUS_H
#define RCC_SYS_SPIBUS_H

#include "Bus.h"

namespace hw
{

struct ChipSelect
{
    GPIO_TypeDef *const gpioType;
    const uint16_t pinNumber;

    ChipSelect() = delete;
    constexpr ChipSelect(GPIO_TypeDef* gpioType, uint16_t pinNumber)
    : gpioType(gpioType), pinNumber(pinNumber) { }
};

class SPIBus : public Bus<SPIHandle>
{
public:
    constexpr explicit SPIBus(SPIHandle* handle) : Bus(handle) { };
    inline void clearCS(const ChipSelect& cs) const;
    inline void setCS(const ChipSelect& cs) const;
    inline BusResult transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const;
    inline BusResult transmitAndReceive(uint8_t* txData, uint8_t* rxData, uint16_t size, uint32_t timeout = defaultTimeout) const;
private:
    static constexpr uint32_t defaultTimeout = 100;
};

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

#endif//RCC_SYS_SPIBUS_H
