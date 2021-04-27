//
// Created by Jakub Zygmunt on 19.12.2020.
//

#ifndef HABSAT_OBC_TERMINAL_H
#define HABSAT_OBC_TERMINAL_H

#include <cstring>
#include <string_view>

#include "Buses/uart.hpp"
#include "System/mutex.hpp"

namespace habsat::terminal {

class Terminal {
   public:
    explicit Terminal(buses::UART& uart) : uart_(uart) {}

    /**
     * Sends the message to the terminal in normal mode
     * @param message
     */
    void pcTransmit(std::string_view message);

    /**
     * Sends the message to the terminal in DMA mode
     * @param message
     */
    void pcTransmitDMA(std::string_view message);

   private:
    buses::UART& uart_;
    system::Mutex mutex_;
};

}  // namespace habsat::terminal

#endif  // HABSAT_OBC_TERMINAL_H
