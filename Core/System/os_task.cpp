//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include "os_task.h"

namespace os
{
    Task::Task(const char *const name, const uint16_t stack_depth, const Priority &priority,
               void (*task_code)(void *), void const* args)
    : m_task_handle(NULL), m_task_code(task_code), m_name(name), m_stack_depth(stack_depth), m_priority(priority),
      m_args(args)
    {

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

    void Task::resume_from_ISR() const
    {
        portYIELD_FROM_ISR(xTaskResumeFromISR(m_task_handle));
    }

    void Task::delete_itself()
    {
        vTaskDelete(NULL);
    }

    void Task::delete_task() const
    {
        vTaskDelete(m_task_handle);
    }

    void Task::addToScheduler() const
    {
        xTaskCreate(m_task_code, m_name, m_stack_depth, const_cast<void *>(m_args),
                    static_cast<int>(m_priority), const_cast<TaskHandle_t *const>(&m_task_handle));
    }
}