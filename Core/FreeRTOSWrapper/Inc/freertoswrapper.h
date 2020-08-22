//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_FREERTOSWRAPPER_H
#define RCC_SYS_FREERTOSWRAPPER_H

#include "FreeRTOS.h"
#include "task.h"
#include <functional>

namespace os
{
    class Task
    {
    private:
        TaskHandle_t m_task_handle;
        std::function<void()> m_task_code;

    public:
        Task(const char * const name, const configSTACK_DEPTH_TYPE usStackDepth, UBaseType_t uxPriority, std::function<void()> task_code);
        void suspend() const;
        void resume() const;
    public:
        static void delay(const uint32_t miliseconds);
        static void delay_until(TickType_t *previous_wake_time, const uint32_t miliseconds);
        static void start_scheduler();
    };

    namespace details
    {
        static void call_task_function(void *args);
    }
};

#endif //RCC_SYS_FREERTOSWRAPPER_H
