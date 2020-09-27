//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "Inc/wifi.h"

size_t WiFi::get_buffer_size()
{
    return m_receive_buffer.size();
}

void WiFi::process_data_from_uart(int start, int end)
{

}

WiFi::WiFi(UART_HandleTypeDef *uart_handle)
: m_uart_handle(uart_handle)
{

}

void WiFi::initialize()
{
    __HAL_UART_ENABLE_IT(m_uart_handle, UART_IT_IDLE);
    HAL_UART_Receive_DMA(m_uart_handle, reinterpret_cast<uint8_t*>(&m_receive_buffer[0]), m_receive_buffer.size());
}
