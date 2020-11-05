//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_OS_TASK_H
#define RCC_SYS_OS_TASK_H

#include "FreeRTOS.h"
#include "task.h"

namespace os
{
    class Task
    {

    public:
        /**
         * Alias for eTaskState enum.
         */
        using State = eTaskState;

        /**
         * Priority of a task.
         */
        enum class Priority
        {
            IDLE = tskIDLE_PRIORITY,
            INTERRUPT = configMAX_SYSCALL_INTERRUPT_PRIORITY
        };

    public:
        /**
         * Task's constructor.
         * @param name          Name of the task.
         * @param stack_depth  Approximate size of the stack needed for the task's function.
         * @param priority      Priority of the task.
         * @param task_code     Pointer to a function that contains the code of the task.
         * @param args          void* used to pass arguments to the task
         */
        Task(const char * const name, const configSTACK_DEPTH_TYPE stack_depth, const Priority &priority,
             void (*const task_code)(void*), void const* args = NULL);

        /**
         * Suspends the task.
         */
        void suspend() const;

        /**
         * Resumes the task.
         */
        void resume() const;

        /**
         * Deletes the task form scheduler.
         */
        void delete_task() const;

        /**
         * Getter for m_task_handle
         */
        TaskHandle_t get_task_handle() const;

        /**
         *
         * @return State from os::Thread::State enum.
         */
        State get_state() const;

        /**
         * Resumes the task from the interrupt handler. To use this you need to create a global pointer
         * to the object and then invoke this function in a interrupt callback function implementation.
         */
        void resume_from_ISR() const;

        /**
         * Allocated memory and adds the task to the scheduler.
         */
        void add_to_scheduler() const;

    public:
        /**
         * Stops the task for no less than the time specified in miliseconds. Note that the delay
         * may last a little bit longer.
         * @param miliseconds
         */
        static void delay(const uint32_t miliseconds);

        /**
         * Stops the task for the exact time specified in miliseconds.
         *
         * @param previous_wake_time
         * @param miliseconds
         */
        static void delay_until(TickType_t *previous_wake_time, const uint32_t miliseconds);

        /**
         * Starts the scheduler. In other words, starts all the tasks.
         */
        static void start_scheduler();

        /**
         * Allows the task to resume itself. This method has to be static as the task's function does not
         * have access to non-static methods, since the object is not yet created.
         */
        static void suspend_itself();

        /**
         * Allows the task to suspend itself. This method has to be static as the task's function does not
         * have access to non-static methods, since the object is not yet created.
         */
        static void resume_itself();

        /**
         * Allows the task to delete itself from scheduler. This method has to be static as the task's function does not
         * have access to non-static methods, since the object is not yet created.
         */
        static void delete_itself();

    private:
        TaskHandle_t m_task_handle;
        void (*const m_task_code)(void*);
        const char *const m_name;
        const configSTACK_DEPTH_TYPE m_stack_depth;
        const Priority m_priority;
        void const* m_args;
    };
};

#endif//RCC_SYS_OS_TASK_H
