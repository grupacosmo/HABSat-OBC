//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_UARTBUS_H
#define HABSAT_OBC_UARTBUS_H

#include <Bus.h>

#include "extern_handles.h"

namespace mcuBoard {

class UARTBus : public Bus<UARTHandle> {
   public:
    explicit UARTBus(UART_HandleTypeDef* handle) : handle_(handle) {}

    auto transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const
          -> BusResult {
        return static_cast<BusResult>(HAL_UART_Transmit(handle_, data, size, timeout));
    }

   private:
    static constexpr uint32_t defaultTimeout = 100;
    UART_HandleTypeDef& handle_;
};

}  // namespace mcuBoard

#endif  // HABSAT_OBC_UARTBUS_H