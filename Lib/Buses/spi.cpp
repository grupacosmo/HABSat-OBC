//
// Created by vixu on 4/6/21.
//

#include "Buses/spi.hpp"

namespace habsat::buses {

using namespace details;

auto SPI::transmit(mcuBoard::GPIOPin& chipSelect, uint8_t data, uint32_t timeout) const -> Result {
    chipSelect.reset();

    const auto result = toResult(HAL_SPI_Transmit(&handle_, &data, 1, timeout));

    chipSelect.set();

    return result;
}

auto SPI::transmit(mcuBoard::GPIOPin& chipSelect, gsl::span<const uint8_t> data, uint32_t timeout)
      const -> Result {
    chipSelect.reset();

    const auto result = toResult(
          HAL_SPI_Transmit(&handle_, const_cast<uint8_t*>(data.data()), data.size(), timeout));

    chipSelect.set();

    return result;
}

auto SPI::transmitAndReceive(
      mcuBoard::GPIOPin& chipSelect,
      gsl::span<const uint8_t> txData,
      gsl::span<uint8_t> rxData,
      uint32_t timeout) const -> Result {
    Expects(txData.size() == rxData.size());

    chipSelect.reset();

    const auto result = toResult(HAL_SPI_TransmitReceive(
          &handle_,
          const_cast<uint8_t*>(txData.data()),
          rxData.data(),
          txData.size(),
          timeout));

    chipSelect.set();

    return result;
}

}  // namespace habsat::buses