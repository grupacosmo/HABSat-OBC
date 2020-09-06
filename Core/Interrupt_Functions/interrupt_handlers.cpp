//
// Created by Wiktor Wieclaw on 9/5/20.
//

#include "interrupt_handlers.h"

void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    int notification = 1;
    if(USART1 == huart->Instance)                                   //Determine whether it is serial port 1
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   //Judging whether it is idle interruption
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                     //Clear idle interrupt sign (otherwise it will continue to enter interrupt)

            uart_notification_queue.send(notification, 0);
        }
    }
}

void USER_UART_DMA_IRQHandler(DMA_HandleTypeDef *hdma_usart_rx)
{
    int notification = 1;
    if(DMA2_Stream2 == hdma_usart_rx->Instance)
    {
        if(RESET != __HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))   //Judging whether it is idle interruption
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);//Clear idle interrupt sign (otherwise it will continue to enter interrupt)

            uart_notification_queue.send(notification, 0);
        }
    }
}

// TODO WW: I will use this later
#if 0
            HAL_UART_DMAStop(huart);
            uint8_t data_length  = receive_buffer.size() - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
            lcd.print_line(2, std::to_string(data_length));

            std::string r1(receive_buffer.begin(), receive_buffer.begin() + data_length);
            lcd.print_line(0, r1);

            HAL_UART_Receive_DMA(huart, reinterpret_cast<uint8_t*>(&receive_buffer[0]), receive_buffer.size());
#endif