//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_I2CBUS_H
#define RCC_SYS_I2CBUS_H

#include "Bus.h"
#include <array>
namespace hw
{

class I2CBus : public Bus<I2CHandle>
{
public:
    explicit I2CBus(I2CHandle* handle);
    BusResult transmit(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout = defaultTimeout) const;
    BusResult memoryRead(uint16_t slaveAddress, uint16_t memoryAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout = defaultTimeout) const;
    BusResult memoryWrite(uint16_t slaveAddress, uint16_t memoryAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout = defaultTimeout) const;
private:
    constexpr uint16_t addressSize(uint16_t memoryAddress) const;
private:
    static constexpr uint32_t defaultTimeout = 100;
};

}
#endif//RCC_SYS_I2CBUS_H
