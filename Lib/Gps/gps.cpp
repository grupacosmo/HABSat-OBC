//
// Created by Filip on 20.04.2021.
//

#include "Inc/Gps/gps.hpp"

#include <terminal.hpp>
#include <vector>
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

    uint8_t data_length  = rawBufferSize - __HAL_DMA_GET_COUNTER(&dmaRx);

    rawMessageBuffor = std::string(reinterpret_cast<char*>(rawBuffor));
    rawMessageBuffor = rawMessageBuffor.substr(0,data_length);

    GpsMessages = retrieveSingleMessage(rawMessageBuffor);

    //todo debug show messages
        Terminal::pcTransmit("---  Gps Messages  ---\r\n");
        Terminal::pcTransmit(GpsMessages.Rmc+"\r\n");
        Terminal::pcTransmit(GpsMessages.Vtg+"\r\n");
        Terminal::pcTransmit(GpsMessages.Gga+"\r\n");
        Terminal::pcTransmit(GpsMessages.Gsa+"\r\n");
        Terminal::pcTransmit(GpsMessages.Gsv+"\r\n");
        Terminal::pcTransmit(GpsMessages.Gll+"\r\n");

    //transmission restart
    HAL_UART_Receive_DMA(&uart, rawBuffor, rawBufferSize);
}

auto Gps::retrieveSingleMessage(std::string & rawMessage) -> GpsData {
    std::string m_RMC, m_VTG, m_GGA, m_GSA, m_GSV, m_GLL;
    std::array<char, 2200> memoryBuffer{};

    while(!rawMessage.empty()){
        auto endOfMessage = rawMessage.find("\r\n");
        //TODO check max message size and change to std::array
        std::string tempMessage = rawMessage.substr(0,endOfMessage);
        auto type = tempMessage.substr(3,3);

        if(type == "RMC") {
            m_RMC = tempMessage;
            //debug
            processRmcMessage(tempMessage);
            std::sprintf(memoryBuffer.data(),
                   "$GPRMC,%hhu%hhu%hhu.%3hd,%c,%f,%c,%f,%c,%lf,%lf,%2hhd%2hhd%2hhd,\r\n",
                   gpsBuffer.utcHour,
                   gpsBuffer.utcMin,
                   gpsBuffer.utcSec,
                   gpsBuffer.utcMicroSec,
                   gpsBuffer.rmcStatus,
                   gpsBuffer.latitude,
                   gpsBuffer.nsIndicator,
                   gpsBuffer.longitude,
                   gpsBuffer.ewIndicator,
                   gpsBuffer.speedOverGroundKmPerH,
                   gpsBuffer.courseOverGroundDegrees,
                   gpsBuffer.day,
                   gpsBuffer.month,
                   gpsBuffer.year
            );
            //Terminal::pcTransmitDMA(memoryBuffer.data());
        }
        else if(type == "VTG") {
            m_VTG = tempMessage;
        }
        else if(type == "GGA") {
            m_GGA = tempMessage;
            //debug
            //processGgaMessage(tempMessage);
            /*std::sprintf(memoryBuffer.data(),
                         "UTC: %hhu-%hhu-%hhu.%3hu\n\r",
                         gpsBuffer.utcHour,
                         gpsBuffer.utcMin,
                         gpsBuffer.utcSec,
                         gpsBuffer.utcMicroSec
            );*/
            //Terminal::pcTransmitDMA(memoryBuffer.data());
        }
        else if(type == "GSA") {
            m_GSA = tempMessage;
        }
        else if(type == "GSV") {
            m_GSV = tempMessage;
        }
        else if(type == "GLL") {
            m_GLL = tempMessage;
        }
        rawMessage.erase(0, endOfMessage+2);
    }
        GpsData result{
                m_RMC,m_VTG,m_GGA,m_GSA,m_GSV,m_GLL
        };
        return result;
}

auto Gps::getData() -> GpsData {
    return GpsMessages;
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
