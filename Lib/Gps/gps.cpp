//
// Created by Filip on 20.04.2021.
//

#include "Inc/Gps/gps.hpp"

#include <terminal.hpp>

namespace habsat::gps{

Gps::Gps(UART_HandleTypeDef *uart, DMA_HandleTypeDef* dma_usart_rx) : uart(uart), dma_rx(dma_usart_rx){
    init();
    defaultConfig();
}

void gps::Gps::init() {
    __HAL_UART_ENABLE_IT(uart, UART_IT_IDLE);
    HAL_UART_Receive_DMA(uart, rawBuffor, rawBufferSize);
}
void gps::Gps::callback() {

    HAL_UART_DMAStop(uart);

    uint8_t data_length  = rawBufferSize - __HAL_DMA_GET_COUNTER(dma_rx);

    Terminal::pcTransmit(reinterpret_cast<char*>(rawBuffor));



    //Restart of transmition
    HAL_UART_Receive_DMA(uart, (uint8_t*)rawBuffor, rawBufferSize);
}
void Gps::defaultConfig() {


}

} //namespace habsat::gps
