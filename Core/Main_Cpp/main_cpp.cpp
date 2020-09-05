//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_cpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "freertoswrapper.h"
#include "idle_tasks.h"
#include "interrupt_tasks.h"
#include "interrupt_callbacks.h"

/* global variables */
const Led led;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
constexpr uint8_t LCD_SLAVE_ADDRESS = 0x4E;
Lcd lcd(4, 20, &hi2c1, LCD_SLAVE_ADDRESS);

/* interrupt callbacks */
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* idle tasks */
const os::Task led_task("lcd_task", 128, os::Task::Priority::IDLE, led_task_code);
const os::Task lcd_task("lcd_task", 128, os::Task::Priority::IDLE, lcd_task_code);

/* interrupt tasks */
const os::Task button_interrupt_task("button_task", 128, os::Task::Priority::INTERRUPT, button_interrupt_task_code);

void main_cpp()
{
    lcd.initialize();

    led_task.add_to_scheduler();
    lcd_task.add_to_scheduler();

    button_interrupt_task.add_to_scheduler();

#define TEST 1
#if TEST

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, reinterpret_cast<uint8_t*>(&receive_buffer[0]), receive_buffer.size());

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
    os::Task::start_scheduler();
}
