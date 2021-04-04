//
// Created by Jakub Zygmunt on 19.12.2020.
//

#ifndef HABSAT_OBC_TERMINAL_H
#define HABSAT_OBC_TERMINAL_H

#include <cstring>
#include <string_view>

#include "Buses/uart.hpp"

namespace habsat {

class Terminal {
   public:
    Terminal() = delete;

    /**
     * Sends the message to the terminal in normal mode
     * @param message
     */
    static void pcTransmit(std::string_view message);

    /**
     * Sends the message to the terminal in DMA mode
     * @param message
     */
    static void pcTransmitDMA(std::string_view message);

    /**
     * TODO if someone needs it
     * @param message
     */
    static void pcReceiveDma(char* message);
};

}  // namespace habsat

#endif  // HABSAT_OBC_TERMINAL_H
