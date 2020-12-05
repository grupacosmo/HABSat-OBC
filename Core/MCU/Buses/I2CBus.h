//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_I2CBUS_H
#define HABSAT_OBC_I2CBUS_H

#include <array>

#include "Bus.h"

namespace hw {

class I2CBus : public Bus<I2CHandle> {
 public:
  constexpr explicit I2CBus(I2CHandle* handle) : Bus(handle) {}

  inline auto transmit(uint16_t slaveAddress, uint8_t data,
                       uint32_t timeout = defaultTimeout_) const -> BusResult;

  template <size_t size>
  inline auto transmit(uint16_t slaveAddress, const std::array<uint8_t, size>& data,
                       uint32_t timeout = defaultTimeout_) const -> BusResult;

  template <uint16_t memoryAddress, size_t size>
  inline auto memoryRead(uint16_t slaveAddress, std::array<uint8_t, size>& data,
                         uint32_t timeout = defaultTimeout_) const -> BusResult;

  template <uint16_t memoryAddress, size_t size>
  inline auto memoryWrite(uint16_t slaveAddress, const std::array<uint8_t, size>& data,
                          uint32_t timeout = defaultTimeout_) const -> BusResult;

 private:
  static constexpr auto addressSizeOption(uint16_t memoryAddress) -> uint16_t;

 private:
  static constexpr uint32_t defaultTimeout_ = 100;
};

auto I2CBus::transmit(uint16_t slaveAddress, uint8_t data, uint32_t timeout) const -> BusResult {
  const auto result = HAL_I2C_Master_Transmit(handle_, slaveAddress, &data, 1, timeout);
  return static_cast<BusResult>(result);
}

template <size_t size>
auto I2CBus::transmit(uint16_t slaveAddress, const std::array<uint8_t, size>& data,
                      uint32_t timeout) const -> BusResult {
  const auto result = HAL_I2C_Master_Transmit(handle_, slaveAddress,
                                              const_cast<uint8_t*>(data.data()), size, timeout);
  return static_cast<BusResult>(result);
}

template <uint16_t memoryAddress, size_t size>
auto I2CBus::memoryRead(uint16_t slaveAddress, std::array<uint8_t, size>& data,
                        uint32_t timeout) const -> BusResult {
  constexpr auto addressSize = addressSizeOption(memoryAddress);

  const auto result = HAL_I2C_Mem_Read(handle_, slaveAddress, memoryAddress, addressSize,
                                       data.data(), size, timeout);
  return static_cast<BusResult>(result);
}

template <uint16_t memoryAddress, size_t size>
auto I2CBus::memoryWrite(uint16_t slaveAddress, const std::array<uint8_t, size>& data,
                         uint32_t timeout) const -> BusResult {
  constexpr auto addressSize = addressSizeOption(memoryAddress);

  const auto result = HAL_I2C_Mem_Write(handle_, slaveAddress, memoryAddress, addressSize,
                                        const_cast<uint8_t*>(data.data()), size, timeout);
  return static_cast<BusResult>(result);
}

constexpr auto I2CBus::addressSizeOption(uint16_t memoryAddress) -> uint16_t {
  if ((memoryAddress & 0xFF00) == 0)
    return 1;
  else
    return 2;
}

}  // namespace hw
#endif  // HABSAT_OBC_I2CBUS_H
