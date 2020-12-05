//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_SPIBUS_H
#define HABSAT_OBC_SPIBUS_H

#include <array>

#include "Bus.h"
#include "GPIOPin.h"

namespace hw {

using ChipSelect = GPIOPin;

class SPIBus : public Bus<SPIHandle> {
 public:
  constexpr explicit SPIBus(SPIHandle* handle) : Bus(handle){};

  inline auto transmit(ChipSelect& cs, uint8_t data, uint32_t timeout = defaultTimeout_) const
      -> BusResult;

  template <size_t size>
  inline auto transmit(ChipSelect& cs, const std::array<uint8_t, size>& data,
                       uint32_t timeout = defaultTimeout_) const -> BusResult;

  template <size_t size>
  inline auto transmitAndReceive(ChipSelect& cs, const std::array<uint8_t, size>& txData,
                                 std::array<uint8_t, size>& rxData,
                                 uint32_t timeout = defaultTimeout_) const -> BusResult;

  template <size_t size>
  inline auto transmitAndReceive(ChipSelect& cs, std::array<uint8_t, size>& data,
                                 uint32_t timeout = defaultTimeout_) const -> BusResult;

 private:
  static constexpr uint32_t defaultTimeout_ = 100;
};

auto SPIBus::transmit(ChipSelect& cs, uint8_t data, uint32_t timeout) const -> BusResult {
  cs.reset();
  const auto result = HAL_SPI_Transmit(handle_, &data, 1, timeout);
  cs.set();
  return static_cast<BusResult>(result);
}

template <size_t size>
auto SPIBus::transmit(ChipSelect& cs, const std::array<uint8_t, size>& data, uint32_t timeout) const
    -> BusResult {
  cs.reset();
  const auto result = HAL_SPI_Transmit(handle_, const_cast<uint8_t*>(data.data()), size, timeout);
  cs.set();
  return static_cast<BusResult>(result);
}

template <size_t size>
auto SPIBus::transmitAndReceive(ChipSelect& cs, const std::array<uint8_t, size>& txData,
                                std::array<uint8_t, size>& rxData, uint32_t timeout) const
    -> BusResult {
  cs.reset();
  const auto result = HAL_SPI_TransmitReceive(handle_, const_cast<uint8_t*>(txData.data()),
                                              rxData.data(), size, timeout);
  cs.set();
  return static_cast<BusResult>(result);
}

template <size_t size>
auto SPIBus::transmitAndReceive(ChipSelect& cs, std::array<uint8_t, size>& data,
                                uint32_t timeout) const -> BusResult {
  return transmitAndReceive(cs, data, data, timeout);
}

}  // namespace hw

#endif  // HABSAT_OBC_SPIBUS_H
