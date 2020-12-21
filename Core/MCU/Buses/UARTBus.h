//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_UARTBUS_H
#define HABSAT_OBC_UARTBUS_H

#include <Bus.h>

namespace hw {

class UARTBus : public Bus<UARTHandle> {
 public:
  constexpr explicit UARTBus(UARTHandle* handle) : Bus(handle) {}
  inline auto transmit(uint8_t* data, uint16_t size, uint32_t timeout = defaultTimeout) const
      -> BusResult;
  inline auto transmitDMA(uint8_t* data, uint16_t size) const -> BusResult;
  inline auto receiveDMA(uint8_t* data, uint16_t size) const -> BusResult;

 private:
  static constexpr uint32_t defaultTimeout = 100;
};

auto UARTBus::transmit(uint8_t* data, uint16_t size, uint32_t timeout) const -> BusResult {
  return static_cast<BusResult>(HAL_UART_Transmit(handle_, data, size, timeout));
}

auto UARTBus::transmitDMA(uint8_t* data, uint16_t size) const -> BusResult {
  return static_cast<BusResult>(HAL_UART_Transmit_DMA(handle_, data, size));
}

auto UARTBus::receiveDMA(uint8_t* data, uint16_t size) const -> BusResult {
  return static_cast<BusResult>(HAL_UART_Receive_DMA(handle_, data, size));
}

}  // namespace hw

#endif  // HABSAT_OBC_UARTBUS_H