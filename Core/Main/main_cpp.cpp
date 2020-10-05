//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_cpp.h"

#include "interrupt_handlers.h"
#include "lcd_tasks.h"
#include "led_tasks.h"
#include "utils.h"

Peripherals peripherals;
Utils utils;

void main_cpp()
{
    peripherals.initialize();
    utils.button.task.add_to_scheduler();
    led_task.add_to_scheduler();
    lcd_task.add_to_scheduler();

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