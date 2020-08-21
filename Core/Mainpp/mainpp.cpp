//
// Created by Wiktor Wieclaw on 8/6/20.
//



#include "mainpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "task.h"
#include "button.h"
#include "freertoswrapper.h"

constexpr uint8_t lcd_slave_address = 0x4E;
extern I2C_HandleTypeDef hi2c1;

void mainpp()
{
    Button button;
    Led led;
    Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    std::function button_led_task = [button, led]()
    {
        while(true)
        {
            vTaskDelay(pdMS_TO_TICKS(1));
            if (button.is_pressed())
            {
                led.toggle();
            }
            while (button.is_pressed());
        }
    };

    os::create_task(button_led_task, "button_led", 128, configMAX_PRIORITIES - 1, NULL);

    std::function lcd_task_test = [lcd]()
    {
        const int delay_ms = 1111;
        while(true)
        {
            lcd.print_line(0, "display");
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
            lcd.print_line(0, "test");
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
        }
    };

    os::create_task(lcd_task_test, "lcd", 128, configMAX_PRIORITIES - 1, NULL);

    vTaskStartScheduler();
    while(true);
}
