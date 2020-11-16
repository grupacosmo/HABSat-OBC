//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_UARTBUS_H
#define RCC_SYS_UARTBUS_H

#include <Bus.h>

namespace hw
{

class UARTBus : public Bus<UARTHandle>
{
public:
    explicit UARTBus(UARTHandle* handle);
    BusResult transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const;
private:
    static constexpr uint32_t defaultTimeout = 100;
};

}

#endif//RCC_SYS_UARTBUS_H