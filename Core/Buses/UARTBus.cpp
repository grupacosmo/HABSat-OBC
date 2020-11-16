//
// Created by Wiktor Wieclaw on 11/16/20.
//

#include "UARTBus.h"

namespace hw
{

UARTBus::UARTBus(UARTHandle *handle) : Bus(handle)
{

}
BusResult UARTBus::transmit(uint8_t* data, uint16_t size, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_UART_Transmit(handle, data, size, timeout));
}

}