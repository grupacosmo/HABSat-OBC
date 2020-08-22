//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include "freertoswrapper.h"

namespace os
{
    Task::Task(const char *const name, const uint16_t usStackDepth, UBaseType_t uxPriority, std::function<void()> task_code)
    : m_task_handle(NULL), m_task_code(task_code)
    {
        xTaskCreate(call_task_function, name, usStackDepth, static_cast<void *>(&m_task_code),
                    uxPriority, &m_task_handle);
    }

    void Task::delay(const uint32_t miliseconds)
    {
        vTaskDelay(pdMS_TO_TICKS(miliseconds));
    }

    void Task::delay_until(TickType_t *previous_wake_time, const uint32_t miliseconds)
    {
        vTaskDelayUntil(previous_wake_time, pdMS_TO_TICKS(miliseconds));
    }

    void Task::start_scheduler()
    {
        vTaskStartScheduler();
    }

    void Task::suspend() const
    {
        vTaskSuspend(m_task_handle);
    }

    void Task::resume() const
    {
        vTaskResume(m_task_handle);
    }

    void Task::call_task_function(void *args)
    {
        (*static_cast<std::function<void()> *>(args))();
    }

    void Task::suspend_itself()
    {
        vTaskSuspend(NULL);
    }

    void Task::resume_itself()
    {
        vTaskResume(NULL);
    }

    Task::State Task::get_state() const
    {
        return eTaskGetState(m_task_handle);
    }

    TaskHandle_t Task::get_task_handle() const
    {
        return m_task_handle;
    }
}