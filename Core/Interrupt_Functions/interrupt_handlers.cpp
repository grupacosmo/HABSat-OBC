//
// Created by Wiktor Wieclaw on 9/5/20.
//

#include "Inc/interrupt_handlers.h"

void USER_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if(USART1 == huart->Instance)                                   //Determine whether it is serial port 1
    {
        if(RESET != __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))   //Judging whether it is idle interruption
        {
            __HAL_UART_CLEAR_IDLEFLAG(huart);                     //Clear idle interrupt sign (otherwise it will continue to enter interrupt)

            HAL_UART_DMAStop(huart);

            uint8_t data_length  = receive_buffer.size() - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
            lcd.print_line(2, std::to_string(data_length));

            std::string r1(receive_buffer.begin(), receive_buffer.begin() + data_length);
            lcd.print_line(0, r1);

            HAL_UART_Receive_DMA(huart, reinterpret_cast<uint8_t*>(&receive_buffer[0]), receive_buffer.size());
        }
    }
}

void USER_UART_DMA_IRQHandler(DMA_HandleTypeDef *hdma_usart_rx)
{

}