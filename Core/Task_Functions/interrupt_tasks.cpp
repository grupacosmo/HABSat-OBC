//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "interrupt_tasks.h"

void button_interrupt_task_code(void *args)
{
    while(true)
    {
        os::Task::suspend_itself();
        lcd.print_line(3, "click");
        if(led_task.get_state() != os::Task::State::eSuspended)
            led_task.suspend();
        else
            led_task.resume();
        os::Task::delay(1000);
        lcd.print_line(3, "");
    }
}

void uart_receive_interrupt_task_code(void *args)
{
    size_t position_old = 0;
    size_t position_current;

    int notification;
    while(true)
    {
        // TODO: this may wake up after 50 days without notifications
        uart_notification_queue.receive(notification, portMAX_DELAY);

        position_current = wifi.get_buffer_size() - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

        if(position_current != position_old)
        {
            if (position_current > position_old)
            {
                wifi.process(position_old, position_current - position_old);
            }
            else
            {
                wifi.process(position_old, wifi.get_buffer_size() - position_old);
                if(position_current > 0)
                {
                    wifi.process(0, position_current);
                }
            }
            position_old = position_current;
        }
        if(position_old == wifi.get_buffer_size())
            position_old = 0;
    }
}
