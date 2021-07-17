//
// Created by Filip on 20.04.2021.
//

#include "Inc/Gps/gps.hpp"

#include <cstdio>
#include <terminal.hpp>

namespace habsat::gps {

Gps::Gps(UART_HandleTypeDef& uart, DMA_HandleTypeDef& dma_usart_rx)
    : uart(uart), dmaRx(dma_usart_rx) {
    init();
}

void gps::Gps::init() {
    __HAL_UART_ENABLE_IT(&uart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&uart, reinterpret_cast<uint8_t*>(rawBuffor), rawBufferSize);
}

void gps::Gps::callback() {
    HAL_UART_DMAStop(&uart);

    rawMessageBuffor = std::string(reinterpret_cast<char*>(rawBuffor));

    gpsMessages = GpsMessages{rawMessageBuffor};

    gpsData = GpsData{gpsMessages};

    //todo remove, printing for debug
    gpsData.print();

    // transmission restart
    HAL_UART_Receive_DMA(&uart, rawBuffor, rawBufferSize);
}

auto Gps::getData() -> GpsData { return gpsData; }

}  // namespace habsat::gps
