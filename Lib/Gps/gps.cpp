//
// Created by Filip on 20.04.2021.
//

#include "Inc/Gps/gps.hpp"

#include <terminal.hpp>
#include <cstdio>

namespace habsat::gps{

Gps::Gps(UART_HandleTypeDef& uart, DMA_HandleTypeDef& dma_usart_rx) : uart(uart), dmaRx(dma_usart_rx){
    init();
}

void gps::Gps::init() {
    __HAL_UART_ENABLE_IT(&uart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&uart, reinterpret_cast<uint8_t*>(rawBuffor), rawBufferSize);
}

void gps::Gps::callback() {
    HAL_UART_DMAStop(&uart);

    //uint8_t data_length  = rawBufferSize - __HAL_DMA_GET_COUNTER(&dmaRx);

    rawMessageBuffor = std::string(reinterpret_cast<char*>(rawBuffor));
    //rawMessageBuffor = rawMessageBuffor.substr(0,data_length);

    gpsMessages = GpsMessages{rawMessageBuffor};

    gpsMessages.print();

    //transmission restart
    HAL_UART_Receive_DMA(&uart, rawBuffor, rawBufferSize);
}



auto Gps::getData() -> GpsMessages {
    return gpsMessages;
}

void Gps::processGgaMessage(std::string_view rawMessage) {
    std::sscanf(rawMessage.data(),
           "$GPGGA,%2hhd%2hhd%2hhd.%3hd,\r\n",
           &gpsBuffer.utcHour,
           &gpsBuffer.utcMin,
           &gpsBuffer.utcSec,
           &gpsBuffer.utcMicroSec
    );
}

void Gps::processRmcMessage(std::string_view rawMessage) {
    std::sscanf(rawMessage.data(),
           "$GPRMC,%2hhd%2hhd%2hhd.%3hd,%c,%f,%c,%f,%c,%lf,%lf,%2hhd%2hhd%2hhd",
           &gpsBuffer.utcHour,
           &gpsBuffer.utcMin,
           &gpsBuffer.utcSec,
           &gpsBuffer.utcMicroSec,
           &gpsBuffer.rmcStatus,
           &gpsBuffer.latitude,
           &gpsBuffer.nsIndicator,
           &gpsBuffer.longitude,
           &gpsBuffer.ewIndicator,
           &gpsBuffer.speedOverGroundKmPerH,
           &gpsBuffer.courseOverGroundDegrees,
           &gpsBuffer.day,
           &gpsBuffer.month,
           &gpsBuffer.year
    );
}

} //namespace habsat::gps
