//
// Created by Jakub Zygmunt on 19.12.2020.
//

#include "terminal.hpp"

#include <gsl/span>

#include "obc.hpp"

namespace habsat {

void Terminal::pcTransmit(std::string_view message) {
    getObc().uart.transmit(gsl::span<const uint8_t>{
          reinterpret_cast<const uint8_t*>(message.data()),
          message.size()});
}

void Terminal::pcTransmitDMA(std::string_view message) {
    getObc().uart.transmitDMA(gsl::span<const uint8_t>{
          reinterpret_cast<const uint8_t*>(message.data()),
          message.size()});
}

void Terminal::pcReceiveDma(char* message) {
    // todo
    (void)message;
}

}  // namespace habsat