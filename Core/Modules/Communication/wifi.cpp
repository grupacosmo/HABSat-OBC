//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "Inc/wifi.h"

/*
 * void uart_receive(void *args)
        {
            size_t position_old = 0;
            size_t position_current;
            size_t dma_buffer_size = g_peripherals.wifi.get_buffer_size();

            int notification;
            while (true)
            {
                uart_notification_queue.receive(notification, portMAX_DELAY);

                position_current = dma_buffer_size - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

                if (position_current != position_old)
                {
                    if (position_current > position_old)
                    {
                        g_peripherals.wifi.process_data_from_uart(position_old, position_current - position_old);
                    }
                    else
                    {
                        g_peripherals.wifi.process_data_from_uart(position_old, dma_buffer_size - position_old);
                        if (position_current > 0)
                        {
                            g_peripherals.wifi.process_data_from_uart(0, position_current);
                        }
                    }
                    position_old = position_current;
                }
                if (position_old == dma_buffer_size)
                    position_old = 0;
            }
 */

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

void WiFi::send_cmd(const std::string &s)
{
    HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(const_cast<char*>(&s[0])), s.size(), 100);
}
