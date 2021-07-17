//
// Created by Filip on 20.04.2021.
//

#ifndef HABSAT_OBC_GPS_H
#define HABSAT_OBC_GPS_H

#include <vector>

#include "Buses/uart.hpp"
#include "gpsMessages.hpp"
#include "gpsData.h"

namespace habsat::gps {

class Gps {
   public:
    Gps(UART_HandleTypeDef& uart, DMA_HandleTypeDef& dma_usart_rx);
    void init();
    void callback();
    auto getData() -> GpsData;

   private:
    static const int rawBufferSize = 510;   //82 chars per message + 2 new line chars per message
    UART_HandleTypeDef& uart;
    DMA_HandleTypeDef& dmaRx;
    std::string rawMessageBuffor;
    uint8_t rawBuffor[rawBufferSize];

    GpsMessages gpsMessages;
    GpsData gpsData;
};

}  // namespace habsat::gps

#endif  // HABSAT_OBC_GPS_H
