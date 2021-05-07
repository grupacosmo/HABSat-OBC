//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_SPI_HPP
#define HABSAT_OBC_BUSES_SPI_HPP

#include <stm32f4xx.h>

#include <array>
#include <gsl/assert>
#include <gsl/span>

#include "MCU_Board/gpio_pin.hpp"
#include "result.hpp"

namespace habsat::buses {

class SPI {
   public:
    explicit SPI(SPI_HandleTypeDef& handle) : handle_(handle){};

    auto transmit(
          mcuBoard::GPIOPin& chipSelect,
          uint8_t data,
          uint32_t timeout = defaultTimeout_) const -> Result;

    auto transmit(
          mcuBoard::GPIOPin& chipSelect,
          gsl::span<const uint8_t> data,
          uint32_t timeout = defaultTimeout_) const -> Result;

    auto transmitAndReceive(
          mcuBoard::GPIOPin& chipSelect,
          gsl::span<const uint8_t> txData,
          gsl::span<uint8_t> rxData,
          uint32_t timeout = defaultTimeout_) const -> Result;

   private:
    static constexpr uint32_t defaultTimeout_ = 100;
    SPI_HandleTypeDef& handle_;
};

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_SPI_HPP
