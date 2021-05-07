//
// Created by Jakub Zygmunt on 19.12.2020.
//

#include "Terminal/terminal.hpp"

#include "Buses/uart.hpp"
#include "Utils/to_bytes.hpp"

namespace habsat::terminal {

void Terminal::pcTransmit(std::string_view message) {
    system::LockMutex lock{mutex_, 5};
    uart_.transmit({reinterpret_cast<const uint8_t*>(message.data()), message.size()});
}

void Terminal::pcTransmitDMA(std::string_view message) {
    system::LockMutex lock{mutex_, 5};
    uart_.transmitDMA({reinterpret_cast<const uint8_t*>(message.data()), message.size()});
}

}  // namespace habsat::terminal