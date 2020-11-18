//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_OS_TASK_H
#define RCC_SYS_OS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

namespace os {
/**
 * Priority of a task.
 */
enum class Priority
{
    IDLE = tskIDLE_PRIORITY,
    INTERRUPT = configMAX_SYSCALL_INTERRUPT_PRIORITY
};

class Task
{

public:
    /**
     * Alias for eTaskState enum.
     */
    using State = eTaskState;

public:
    /**
     * Task's constructor.
     * @param name          Name of the task.
     * @param stack_depth  Approximate size of the stack needed for the task's function.
     * @param priority      Priority of the task.
     * @param task_code     Pointer to a function that contains the code of the task.
     * @param args          void* used to pass arguments to the task
     */
    constexpr Task(const char * const name, const configSTACK_DEPTH_TYPE stack_depth, const Priority &priority,
         void (*const task_code)(void*), void const* args = nullptr);

    /**
     * Suspends the task.
     */
    inline void suspend() const;

    /**
     * Resumes the task.
     */
    inline void resume() const;

    /**
     * Deletes the task form scheduler.
     */
    inline void delete_task() const;

    /**
     *
     * @return State from os::Thread::State enum.
     */
    inline State get_state() const;

    /**
     * Resumes the task from the interrupt handler. To use this you need to create a global pointer
     * to the object and then invoke this function in a interrupt callback function implementation.
     */
    inline void resume_from_ISR() const;

    /**
     * Allocated memory and adds the task to the scheduler.
     */
    inline void addToScheduler() const;

public:
    /**
     * Stops the task for no less than the time specified in miliseconds. Note that the delay
     * may last a little bit longer.
     * @param miliseconds
     */
    inline static void delay(const uint32_t miliseconds);

    /**
     * Stops the task for the exact time specified in miliseconds.
     *
     * @param previous_wake_time
     * @param miliseconds
     */
    inline static void delay_until(TickType_t *previous_wake_time, const uint32_t miliseconds);

    /**
     * Starts the scheduler. In other words, starts all the tasks.
     */
    inline static void start_scheduler();

    /**
     * Allows the task to resume itself. This method has to be static as the task's function does not
     * have access to non-static methods, since the object is not yet created.
     */
    inline static void suspend_itself();

    /**
     * Allows the task to suspend itself. This method has to be static as the task's function does not
     * have access to non-static methods, since the object is not yet created.
     */
    inline static void resume_itself();

    /**
     * Allows the task to delete itself from scheduler. This method has to be static as the task's function does not
     * have access to non-static methods, since the object is not yet created.
     */
    inline static void delete_itself();

private:
    const TaskHandle_t m_task_handle;
    void (*const m_task_code)(void*);
    const char *const m_name;
    const configSTACK_DEPTH_TYPE m_stack_depth;
    const Priority m_priority;
    void const* m_args;
};

constexpr Task::Task(const char *const name, const uint16_t stack_depth, const Priority &priority,
           void (*task_code)(void *), void const* args)
    : m_task_handle(NULL), m_task_code(task_code), m_name(name), m_stack_depth(stack_depth),
      m_priority(priority), m_args(args)
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
                static_cast<int>(m_priority), const_cast<TaskHandle_t *>(&m_task_handle));
}

void Task::start_scheduler()
{
    vTaskStartScheduler();
}

}

#endif//RCC_SYS_OS_TASK_H