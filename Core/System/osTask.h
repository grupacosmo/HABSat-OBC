//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_OSTASK_H
#define RCC_SYS_OSTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include <Noncopyable.h>

namespace os
{

/**
 * Priority of a task.
 */
enum class Priority
{
    Idle = tskIDLE_PRIORITY,
    Interrupt = configMAX_SYSCALL_INTERRUPT_PRIORITY
};

enum class TaskState
{
    Running   = eRunning,
    Ready     = eReady,
    Blocked   = eBlocked,
    Suspended = eSuspended,
    Deleted   = eDeleted,
    Invalid   = eInvalid
};

class Task : public Noncopyable
{

public:
    /**
     * Alias for eTaskState enum.
     */
    using StackDepth = configSTACK_DEPTH_TYPE;

public:
    /**
     * Task's constructor.
     * @param name          Name of the task.
     * @param stackDepth  Approximate size of the stack needed for the task's function.
     * @param priority      Priority of the task.
     * @param taskCode     Pointer to a function that contains the code of the task.
     * @param args          void* used to pass arguments to the task
     */
    constexpr Task(const char * const name, const StackDepth stackDepth, const Priority &priority,
         void (*const taskCode)(void*));

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
    inline void deleteTask() const;

    /**
     *
     * @return State from os::Thread::State enum.
     */
    inline TaskState getState() const;

    /**
     * Resumes the task from the interrupt handler. To use this you need to create a global pointer
     * to the object and then invoke this function in a interrupt callback function implementation.
     */
    inline void resumeFromISR() const;

    /**
     * Allocated memory and adds the task to the scheduler.
     */
    inline void addToScheduler(void* params) const;

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
     * @param previousWakeTime
     * @param miliseconds
     */
    inline static void delayUntil(TickType_t *previousWakeTime, const uint32_t miliseconds);

    /**
     * Starts the scheduler. In other words, starts all the tasks.
     */
    inline static void startScheduler();

    /**
     * Allows the task to resume itself. This method has to be static as the task's function does not
     * have access to non-static methods, since the object is not yet created.
     */
    inline static void suspendItself();

    /**
     * Allows the task to suspend itself. This method has to be static as the task's function does not
     * have access to non-static methods, since the object is not yet created.
     */
    inline static void resumeItself();

    /**
     * Allows the task to delete itself from scheduler. This method has to be static as the task's function does not
     * have access to non-static methods, since the object is not yet created.
     */
    inline static void deleteItself();

private:
    const TaskHandle_t taskHandle_;
    void (*const taskCode_)(void*);
    const char *const name_;
    const StackDepth stackDepth_;
    const Priority priority_;
};

constexpr Task::Task(const char *const name, const StackDepth stackDepth, const Priority &priority,
           void (*taskCode)(void *))
    : taskHandle_(NULL), taskCode_(taskCode), name_(name), stackDepth_(stackDepth),
      priority_(priority)
{

}

void Task::delay(const uint32_t miliseconds)
{
    vTaskDelay(pdMS_TO_TICKS(miliseconds));
}

void Task::delayUntil(TickType_t *previousWakeTime, const uint32_t miliseconds)
{
    vTaskDelayUntil(previousWakeTime, pdMS_TO_TICKS(miliseconds));
}

void Task::suspend() const
{
    vTaskSuspend(taskHandle_);
}

void Task::resume() const
{
    vTaskResume(taskHandle_);
}

void Task::suspendItself()
{
    vTaskSuspend(NULL);
}

void Task::resumeItself()
{
    vTaskResume(NULL);
}

TaskState Task::getState() const
{
    return static_cast<TaskState>(eTaskGetState(taskHandle_));
}

void Task::resumeFromISR() const
{
    portYIELD_FROM_ISR(xTaskResumeFromISR(taskHandle_));
}

void Task::deleteItself()
{
    vTaskDelete(NULL);
}

void Task::deleteTask() const
{
    vTaskDelete(taskHandle_);
}

void Task::addToScheduler(void* params = nullptr) const
{
    xTaskCreate(taskCode_, name_, stackDepth_, const_cast<void *>(params),
                static_cast<int>(priority_), const_cast<TaskHandle_t *>(&taskHandle_));
}

void Task::startScheduler()
{
    vTaskStartScheduler();
}

}

#endif//RCC_SYS_OSTASK_H