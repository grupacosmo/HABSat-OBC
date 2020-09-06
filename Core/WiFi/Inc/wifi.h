//
// Created by Wiktor Wieclaw on 8/29/20.
//

#ifndef RCC_SYS_WIFI_H
#define RCC_SYS_WIFI_H

#include "os_queue.h"
#include "stm32f4xx.h"

class WiFi{

public:
    WiFi(UART_HandleTypeDef uart_handle);
    void initialize();
    size_t get_buffer_size();
    void process(int start, int end);
private:
    std::array<char, 255> m_receive_buffer;
    UART_HandleTypeDef m_uart_handle;
};


#endif //RCC_SYS_WIFI_H
