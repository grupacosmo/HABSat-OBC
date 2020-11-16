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
    ChipSelect(GPIO_TypeDef* gpioType, uint16_t pinNumber)
    : gpioType(gpioType), pinNumber(pinNumber) { }
};

class SPIBus : public Bus<SPIHandle>
{
public:
    explicit SPIBus(SPIHandle* handle);
    void clearCS(const ChipSelect& cs) const;
    void setCS(const ChipSelect& cs) const;
    BusResult transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const;
    BusResult transmitAndReceive(uint8_t* txData, uint8_t* rxData, uint16_t size, uint32_t timeout = defaultTimeout) const;
private:
    static constexpr uint32_t defaultTimeout = 100;
};

}

#endif//RCC_SYS_SPIBUS_H
