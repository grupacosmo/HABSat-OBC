//
// Created by Wiktor Wieclaw on 8/29/20.
//

#include "interrupt_tasks.h"

#include "task_objects.h"

/* tasks */
extern global::Tasks g_tasks;

os::Queue<int, 25> uart_notification_queue;

namespace task_code
{
    namespace interrupt
    {
        void button(void *args)
        {
            while (true)
            {
                os::Task::suspend_itself();
                g_peripherals.lcd.print_line(3, "click");
                if (g_tasks.led.get_state() != os::Task::State::eSuspended)
                    g_tasks.led.suspend();
                else
                    g_tasks.led.resume();
                os::Task::delay(1000);
                g_peripherals.lcd.print_line(3, "");
            }
        }

        void uart_receive(void *args)
        {
            size_t position_old = 0;
            size_t position_current;
            size_t dma_buffer_size = g_peripherals.wifi.get_buffer_size();

            int notification;
            while (true)
            {
                uart_notification_queue.receive(notification, portMAX_DELAY);

                position_current = dma_buffer_size - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

                if (position_current != position_old)
                {
                    if (position_current > position_old)
                    {
                        g_peripherals.wifi.process_data_from_uart(position_old, position_current - position_old);
                    }
                    else
                    {
                        g_peripherals.wifi.process_data_from_uart(position_old, dma_buffer_size - position_old);
                        if (position_current > 0)
                        {
                            g_peripherals.wifi.process_data_from_uart(0, position_current);
                        }
                    }
                    position_old = position_current;
                }
                if (position_old == dma_buffer_size)
                    position_old = 0;
            }
        }
    }
}