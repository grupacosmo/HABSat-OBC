//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_I2C_HPP
#define HABSAT_OBC_BUSES_I2C_HPP

#include <stm32f4xx.h>

#include <array>
#include <gsl/span>

#include "result.hpp"

namespace habsat::buses {

class I2C {
   public:
    explicit I2C(I2C_HandleTypeDef& handle) : handle_(handle) {}

    auto transmit(
          uint16_t slaveAddress,
          uint8_t data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        return detail::toResult(
              HAL_I2C_Master_Transmit(&handle_, slaveAddress, &data, 1, timeout));
    }

    auto transmit(
          uint16_t slaveAddress,
          gsl::span<const uint8_t> data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        return detail::toResult(HAL_I2C_Master_Transmit(
              &handle_,
              slaveAddress,
              const_cast<uint8_t*>(data.data()),
              data.size(),
              timeout));
    }

    auto memoryRead(
          uint16_t slaveAddress,
          uint16_t memoryAddress,
          gsl::span<uint8_t> data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        return detail::toResult(HAL_I2C_Mem_Read(
              &handle_,
              slaveAddress,
              memoryAddress,
              addressSizeOption(memoryAddress),
              data.data(),
              data.size(),
              timeout));
    }

    auto memoryWrite(
          uint16_t slaveAddress,
          uint16_t memoryAddress,
          gsl::span<const uint8_t> data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        return detail::toResult(HAL_I2C_Mem_Write(
              &handle_,
              slaveAddress,
              memoryAddress,
              addressSizeOption(memoryAddress),
              const_cast<uint8_t*>(data.data()),
              data.size(),
              timeout));
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
