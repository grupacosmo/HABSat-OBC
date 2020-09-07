//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_cpp.h"

#include "peripherals.h"
#include "task_objects.h"

global::Peripherals g_peripherals;
global::Tasks g_tasks;

void main_cpp()
{
    g_peripherals.initialize();
    g_tasks.initialize();

    os::Task::start_scheduler();
}


#define TEST 0
#if TEST

auto send_cmd = [](const std::string &s)
    {
      HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(const_cast<char*>(&s[0])), s.size(), 100);
      HAL_Delay(5000);
    };

    send_cmd("AT\r\n");
    send_cmd("AT+CWMODE\r\n");

    while(true)
    {

    }
#endif