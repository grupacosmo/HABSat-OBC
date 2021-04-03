//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_I2C_HPP
#define HABSAT_OBC_BUSES_I2C_HPP

#include <stm32f4xx.h>

#include <array>

#include "result.hpp"

namespace habsat::buses {

class I2C {
   public:
    explicit I2C(I2C_HandleTypeDef& handle) : handle_(handle) {}

    auto transmit(uint16_t slaveAddress, uint8_t data, uint32_t timeout = defaultTimeout_) const
          -> Result {
        const auto result = HAL_I2C_Master_Transmit(&handle_, slaveAddress, &data, 1, timeout);
        return static_cast<Result>(result);
    }

    template <size_t size>
    auto transmit(
          uint16_t slaveAddress,
          const std::array<uint8_t, size>& data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        const auto result = HAL_I2C_Master_Transmit(
              &handle_,
              slaveAddress,
              const_cast<uint8_t*>(data.data()),
              size,
              timeout);

        return static_cast<Result>(result);
    }

    template <uint16_t memoryAddress, size_t size>
    auto memoryRead(
          uint16_t slaveAddress,
          std::array<uint8_t, size>& data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        constexpr auto addressSize = addressSizeOption(memoryAddress);

        const auto result = HAL_I2C_Mem_Read(
              &handle_,
              slaveAddress,
              memoryAddress,
              addressSize,
              data.data(),
              size,
              timeout);

        return static_cast<Result>(result);
    }

    template <uint16_t memoryAddress, size_t size>
    auto memoryWrite(
          uint16_t slaveAddress,
          const std::array<uint8_t, size>& data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        constexpr auto addressSize = addressSizeOption(memoryAddress);

        const auto result = HAL_I2C_Mem_Write(
              &handle_,
              slaveAddress,
              memoryAddress,
              addressSize,
              const_cast<uint8_t*>(data.data()),
              size,
              timeout);

        return static_cast<Result>(result);
    }

   private:
    static constexpr auto addressSizeOption(uint16_t memoryAddress) -> uint16_t {
        return (memoryAddress & 0xFF00) == 0 ? 1 : 2;
    }

   private:
    static constexpr uint32_t defaultTimeout_ = 100;
    I2C_HandleTypeDef& handle_;
};

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_I2C_HPP
