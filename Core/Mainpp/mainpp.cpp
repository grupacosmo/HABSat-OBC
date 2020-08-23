//
// Created by Wiktor Wieclaw on 8/6/20.
//

#include "mainpp.h"

#include "stm32f4xx_hal.h"
#include "led.h"
#include "lcd.h"
#include "FreeRTOS.h"
#include "button.h"
#include "task.h"
#include "freertoswrapper.h"

constexpr uint8_t lcd_slave_address = 0x4E;
extern I2C_HandleTypeDef hi2c1;

os::Task *button_interrupt_task_pointer;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    button_interrupt_task_pointer->resume_from_ISR();
}

void mainpp()
{
    Button button;
    Led led;
    Lcd lcd(4, 20, &hi2c1, lcd_slave_address);

    os::Task led_task("lcd_task", 512, tskIDLE_PRIORITY, [led]()
    {
        while(true)
        {
            led.toggle();
            os::Task::delay(1000);
        }
    });


    static os::Task button_interrupt_task("button_interrupt_task", 512, configMAX_SYSCALL_INTERRUPT_PRIORITY, [button, led_task]()
    {
        while(true)
        {
            os::Task::suspend_itself();
            if(led_task.get_state() != os::Task::State::eSuspended)
                led_task.suspend();
            else
                led_task.resume();
        }
    });
    button_interrupt_task_pointer = &button_interrupt_task;

    os::Task lcd_task("lcd_task", 512, tskIDLE_PRIORITY, [lcd]()
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
