//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_SPI_HPP
#define HABSAT_OBC_BUSES_SPI_HPP

#include <stm32f4xx.h>

#include <array>

#include "MCU_Board/gpio_pin.hpp"
#include "result.hpp"

namespace habsat::buses {

class SPI {
   public:
    explicit SPI(SPI_HandleTypeDef& handle) : handle_(handle){};

    auto transmit(mcuBoard::GPIOPin& chipSelect, uint8_t data, uint32_t timeout = defaultTimeout_)
          const -> Result {
        chipSelect.reset();
        const auto result = HAL_SPI_Transmit(&handle_, &data, 1, timeout);
        chipSelect.set();
        return static_cast<Result>(result);
    }

    template <size_t size>
    auto transmit(
          mcuBoard::GPIOPin& chipSelect,
          const std::array<uint8_t, size>& data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        chipSelect.reset();
        const auto result =
              HAL_SPI_Transmit(&handle_, const_cast<uint8_t*>(data.data()), size, timeout);
        chipSelect.set();
        return static_cast<Result>(result);
    }

    template <size_t size>
    auto transmitAndReceive(
          mcuBoard::GPIOPin& chipSelect,
          const std::array<uint8_t, size>& txData,
          std::array<uint8_t, size>& rxData,
          uint32_t timeout = defaultTimeout_) const -> Result {
        chipSelect.reset();
        const auto result = HAL_SPI_TransmitReceive(
              &handle_,
              const_cast<uint8_t*>(txData.data()),
              rxData.data(),
              size,
              timeout);
        chipSelect.set();
        return static_cast<Result>(result);
    }

    template <size_t size>
    auto transmitAndReceive(
          mcuBoard::GPIOPin& chipSelect,
          std::array<uint8_t, size>& data,
          uint32_t timeout = defaultTimeout_) const -> Result {
        return transmitAndReceive(chipSelect, data, data, timeout);
    }

   private:
    static constexpr uint32_t defaultTimeout_ = 100;
    SPI_HandleTypeDef& handle_;
};

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_SPI_HPP
