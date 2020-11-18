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
    constexpr I2CBus(I2CHandle* handle) : Bus(handle) { }
    inline BusResult transmit(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout = defaultTimeout) const;
    template<uint16_t memoryAddress>
    inline BusResult memoryRead(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout = defaultTimeout) const;
    template<uint16_t memoryAddress>
    inline BusResult memoryWrite(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout = defaultTimeout) const;
private:
    static constexpr uint16_t addressSizeOption(uint16_t memoryAddress);
private:
    static constexpr uint32_t defaultTimeout = 100;
};

BusResult I2CBus::transmit(uint16_t slaveAddress, uint8_t *data, uint16_t dataLength, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_I2C_Master_Transmit(handle, slaveAddress, data, dataLength, timeout));
}

template<uint16_t memoryAddress>
BusResult I2CBus::memoryRead(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout) const
{
    constexpr auto addressSize = addressSizeOption(memoryAddress);
    return static_cast<BusResult>(HAL_I2C_Mem_Read(handle, slaveAddress, memoryAddress, addressSize, data, dataLength, timeout));
}

template<uint16_t memoryAddress>
BusResult I2CBus::memoryWrite(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout) const
{
    constexpr auto addressSize = addressSizeOption(memoryAddress);
    return static_cast<BusResult>(HAL_I2C_Mem_Write(handle, slaveAddress, memoryAddress, addressSize, data, dataLength, timeout));
}

constexpr uint16_t I2CBus::addressSizeOption(uint16_t memoryAddress)
{
    if((memoryAddress & 0xFF00) == 0) return 1;
    else return 2;
}

}
#endif//RCC_SYS_I2CBUS_H
