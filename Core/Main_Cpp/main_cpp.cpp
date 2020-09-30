//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "main_cpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "rtc.h"
#include "FreeRTOS.h"
#include "freertoswrapper.h"
#include "idle_tasks.h"
#include "interrupt_tasks.h"
#include "interrupt_callbacks.h"

/* global variables */
const Led led;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
constexpr uint8_t LCD_SLAVE_ADDRESS = 0x4E;
constexpr uint8_t RTC_ADDRESS = 0x68;
Lcd lcd(4, 20, &hi2c1, LCD_SLAVE_ADDRESS);
Rtc rtc(&hi2c3, RTC_ADDRESS);

/* interrupt callbacks */
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

/* idle tasks */
const os::Task led_task("lcd_task", 128, os::Task::Priority::IDLE, led_task_code);
const os::Task lcd_task("lcd_task", 128, os::Task::Priority::IDLE, lcd_task_code);
const os::Task rtc_task("rtc_task", 128, os::Task::Priority::IDLE, rtc_task_code);

/* interrupt tasks */
const os::Task button_interrupt_task("button_task", 128, os::Task::Priority::INTERRUPT, button_interrupt_task_code);

void main_cpp()
{
    rtc.initialize();
    lcd.initialize();

    led_task.add_to_scheduler();
    lcd_task.add_to_scheduler();
    rtc_task.add_to_scheduler();

    button_interrupt_task.add_to_scheduler();

    os::Task::start_scheduler();
}