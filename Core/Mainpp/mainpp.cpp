//
// Created by Wiktor Wieclaw on 8/6/20.
//



#include "mainpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "button.h"
#include "freertoswrapper.h"

constexpr uint8_t lcd_slave_address = 0x4E;
extern I2C_HandleTypeDef hi2c1;

void mainpp()
{
    Button button;
    Led led;
    Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    static os::Task button_interrupt_task("button_interrupt_task",128,configMAX_PRIORITIES-1,[button,led]()
    {
        while(true)
        {
            os::Task::suspend_itself();
            led.toggle();
        }
    });

    os::Task led_task("lcd_task", 128, configMAX_PRIORITIES - 1, [led]()
    {
        while(true)
        {
            led.toggle();
            os::Task::delay(1000);
        }
    });

    os::Task button_task("button_task", 128, configMAX_PRIORITIES - 1, [button, led_task, led]()
    {
        // TODO WW: temporary solution, should be done with interrupt
        while(true)
        {
            if(led_task.get_state() != os::Task::State::eSuspended)
            {
                led_task.suspend();
            }
            else
            {
                led_task.resume();
            }
            while(button.is_pressed());
        }
    });


    os::Task lcd_task("lcd_task", 128, configMAX_PRIORITIES - 1, [lcd]()
    {
        const int delay_ms = 1111;
        while(true)
        {
            lcd.print_line(0, "display");
            os::Task::delay(delay_ms);
            lcd.print_line(0, "test");
            os::Task::delay(delay_ms);
        }
    });

    os::Task::start_scheduler();
    while(true);
}
