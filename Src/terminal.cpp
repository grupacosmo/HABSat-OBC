//
// Created by Jakub Zygmunt on 19.12.2020.
//

#include "terminal.hpp"

#include "obc.hpp"

namespace habsat {

void Terminal::pcTransmit(std::string_view message) {
    obc().uart.transmit(reinterpret_cast<const uint8_t*>(message.data()), message.size());
}

void Terminal::pcTransmitDMA(std::string_view message) {
    obc().uart.transmitDMA(reinterpret_cast<const uint8_t*>(message.data()), message.size());
}

void Terminal::pcReceiveDma(char* message) {
    // todo
    (void)message;
}

}  // namespace habsat