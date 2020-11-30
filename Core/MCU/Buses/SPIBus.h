//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_SPIBUS_H
#define RCC_SYS_SPIBUS_H

#include "Bus.h"
#include "GPIOPin.h"

namespace hw
{

using ChipSelect = GPIOPin;

class SPIBus : public Bus<SPIHandle>
{
public:
    constexpr explicit SPIBus(SPIHandle* handle) : Bus(handle) { };
    inline BusResult transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const;
    inline BusResult transmitAndReceive(uint8_t* txData, uint8_t* rxData, uint16_t size, uint32_t timeout = defaultTimeout) const;
private:
    static constexpr uint32_t defaultTimeout = 100;
};

BusResult SPIBus::transmit(uint8_t *data, uint16_t size, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_SPI_Transmit(handle_, data, size, timeout));
}
BusResult SPIBus::transmitAndReceive(uint8_t *txData, uint8_t *rxData, uint16_t size, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_SPI_TransmitReceive(handle_, txData, rxData, size, timeout));
}

}

#endif//RCC_SYS_SPIBUS_H
