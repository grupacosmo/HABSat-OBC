//
// Created by Wiktor Wieclaw on 11/16/20.
//

#include "I2CBus.h"
namespace hw
{

I2CBus::I2CBus(I2CHandle* handle) : Bus(handle)
{

}
BusResult I2CBus::transmit(uint16_t slaveAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_I2C_Master_Transmit(handle, slaveAddress, data, dataLength, timeout));
}
BusResult I2CBus::memoryRead(uint16_t slaveAddress, uint16_t memoryAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_I2C_Mem_Read(handle, slaveAddress, memoryAddress, addressSize(memoryAddress), data, dataLength, timeout));
}
BusResult I2CBus::memoryWrite(uint16_t slaveAddress, uint16_t memoryAddress, uint8_t* data, uint16_t dataLength, uint32_t timeout) const
{
    return static_cast<BusResult>(HAL_I2C_Mem_Write(handle, slaveAddress, memoryAddress, addressSize(memoryAddress), data, dataLength, timeout));
}
constexpr uint16_t I2CBus::addressSize(uint16_t memoryAddress) const
{
    if((memoryAddress & 0xFF00) == 0) return 1;
    else return 2;
}

}