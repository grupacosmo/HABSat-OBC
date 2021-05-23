//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_UART_HPP
#define HABSAT_OBC_BUSES_UART_HPP

#include <stm32f4xx.h>
#include <gsl/span>

#include "result.hpp"

namespace habsat::buses {

class UART {
   public:
    explicit UART(UART_HandleTypeDef& handle) : handle_(handle) {}

    [[nodiscard]] auto transmit(gsl::span<const uint8_t> data, uint32_t timeout = defaultTimeout) const
          -> Result {
        return detail::toResult(
              HAL_UART_Transmit(&handle_, const_cast<uint8_t*>(data.data()), data.size(), timeout));
    }

    [[nodiscard]] auto transmitDMA(gsl::span<const uint8_t> data) const -> Result {
        return detail::toResult(
              HAL_UART_Transmit_DMA(&handle_, const_cast<uint8_t*>(data.data()), data.size()));
    }

    [[nodiscard]] auto receiveDMA(gsl::span<uint8_t> data) const -> Result {
        return detail::toResult(HAL_UART_Receive_DMA(&handle_, data.data(), data.size()));
    }

   private:
    static constexpr uint32_t defaultTimeout = 100;
    UART_HandleTypeDef& handle_;
};

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_UART_HPP