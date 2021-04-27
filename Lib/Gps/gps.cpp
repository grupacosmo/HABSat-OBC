//
// Created by Filip on 20.04.2021.
//

#include "Inc/Gps/gps.hpp"

#include <terminal.hpp>
#include <vector>

namespace habsat::gps{

Gps::Gps(UART_HandleTypeDef *uart, DMA_HandleTypeDef* dma_usart_rx) : uart(uart), dma_rx(dma_usart_rx){
    init();
}

void gps::Gps::init() {
    __HAL_UART_ENABLE_IT(uart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(uart, reinterpret_cast<uint8_t*>(rawBuffor), rawBufferSize);
}
void gps::Gps::callback() {

    HAL_UART_DMAStop(uart);

    uint8_t data_length  = rawBufferSize - __HAL_DMA_GET_COUNTER(dma_rx);

    rawMessageBuffor = std::string(reinterpret_cast<char*>(rawBuffor));
    rawMessageBuffor = rawMessageBuffor.substr(0,data_length);

    GpsMessages = retriveSingleMessage(rawMessageBuffor);

    //todo debug show messages
    Terminal::pcTransmit("---  Gps Messages  ---\r\n");
        Terminal::pcTransmit(GpsMessages.RMC+"\r\n");
        Terminal::pcTransmit(GpsMessages.VTG+"\r\n");
        Terminal::pcTransmit(GpsMessages.GGA+"\r\n");
        Terminal::pcTransmit(GpsMessages.GSA+"\r\n");
        Terminal::pcTransmit(GpsMessages.GSV+"\r\n");
        Terminal::pcTransmit(GpsMessages.GLL+"\r\n");

    //Restart of transmition
    HAL_UART_Receive_DMA(uart, reinterpret_cast<uint8_t*>(rawBuffor), rawBufferSize);
}
GpsData Gps::retriveSingleMessage(std::string & rawMessage) {

    std::string m_RMC, m_VTG, m_GGA, m_GSA, m_GSV, m_GLL;

    while(!rawMessage.empty()){
        auto endOfMessage = rawMessage.find("\r\n");
        std::string tempMessage = rawMessage.substr(0,endOfMessage);

        switch (matchMessageType(tempMessage)) {
            case RMC:  m_RMC = tempMessage; break;
            case VTG:  m_VTG = tempMessage; break;
            case GGA:  m_GGA = tempMessage; break;
            case GSA:  m_GSA = tempMessage; break;
            case GSV:  m_GSV = tempMessage; break;
            case GLL:  m_GLL = tempMessage; break;
            case None: break;
        }
        rawMessage.erase(0, endOfMessage+2);
    }
        GpsData result{
                m_RMC,m_VTG,m_GGA,m_GSA,m_GSV,m_GLL
        };
        return result;
}


MessageType Gps::matchMessageType(std::string& rawMessage) {

    //auto endOfType = rawMessage.find(',');
    auto type = rawMessage.substr(3,3);

    if("RMC" == type)
        return RMC;
    if("VTG" == type)
        return VTG;
    if("GGA" == type)
        return GGA;
    if("GSA" == type)
        return GSA;
    if("GSV" == type)
        return GSV;
    if("GLL" == type)
        return GLL;

    return None;
}
GpsData Gps::getData() { return GpsMessages; }

} //namespace habsat::gps
