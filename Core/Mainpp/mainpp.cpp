//
// Created by Wiktor Wieclaw on 8/6/20.
//


#include "mainpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"

constexpr uint8_t lcd_slave_address = 0x4E;
extern I2C_HandleTypeDef hi2c1;

Led led;

static void led_task(void *args)
{
    int delay_ms = *(int*)args;

    while(true)
    {
        led.toggle();
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

static void lcd_task(void *args)
{
    int delay_ms = 1111;
    Lcd* lcd = (Lcd*)args;
    while(true)
    {
        lcd->print_line(0, "display");
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        lcd->print_line(0, "test");
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

void mainpp()
{
    static const int led_delay_1 = 1111;
    static const Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    xTaskCreate(led_task, "LED_blink_1", 128, (void*) &led_delay_1, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(lcd_task, "LCD", 128, (void*) &lcd, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();
    while(true);
}
