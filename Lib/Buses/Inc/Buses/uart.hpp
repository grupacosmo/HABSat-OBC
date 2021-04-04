//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_UART_HPP
#define HABSAT_OBC_BUSES_UART_HPP

#include <stm32f4xx.h>

#include "result.hpp"

namespace habsat::buses {

class UART {
   public:
    explicit UART(UART_HandleTypeDef& handle) : handle_(handle) {}

    auto transmit(const uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const
          -> Result {
        return static_cast<Result>(
              HAL_UART_Transmit(&handle_, const_cast<uint8_t*>(data), size, timeout));
    }

    auto transmitDMA(const uint8_t* data, uint16_t size) const -> Result {
        return static_cast<Result>(
              HAL_UART_Transmit_DMA(&handle_, const_cast<uint8_t*>(data), size));
    }

    auto receiveDMA(uint8_t* data, uint16_t size) const -> Result {
        return static_cast<Result>(HAL_UART_Receive_DMA(&handle_, data, size));
    }

   private:
    static constexpr uint32_t defaultTimeout = 100;
    UART_HandleTypeDef& handle_;
};

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_UART_HPP