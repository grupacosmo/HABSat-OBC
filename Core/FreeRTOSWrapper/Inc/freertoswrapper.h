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

    public:
        /**
         * Alias for eTaskState.
         */
        using State = eTaskState;

    public:
        /**
         * Task's constructor.
         * @param name          Name of the task.
         * @param usStackDepth  Approximate size of the stack needed for the task.
         * @param uxPriority    Priority of the task.
         * @param task_code     Lambda that contains the code of the task.
         */
        Task(const char * const name, const configSTACK_DEPTH_TYPE usStackDepth, UBaseType_t uxPriority,
             std::function<void()> task_code);

        /**
         * Suspends the task.
         */
        void suspend() const;

        /**
         * Resumes the task.
         */
        void resume() const;

        /**
         * Getter for m_task_handle
         */
        TaskHandle_t get_task_handle() const;

        /**
         *
         * @return State from os::Thread::State enum.
         */
        State get_state() const;

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
         * Allows the task to suspend itself as the task's function does not have access to
         * non-static methods, since the object is not yet created.
         */
        static void suspend_itself();

        /**
         * Allows the task to resume itself as the task's function does not have access to
         * non-static methods, since the object is not yet created.
         */
        static void resume_itself();


    private:
        /**
         * Calls the function that contains task's code.
         * @param args std::function* casted to void*.
         */
        static void call_task_function(void *task_code);

    private:

        TaskHandle_t m_task_handle;
        std::function<void()> m_task_code;
    };
};

#endif //RCC_SYS_FREERTOSWRAPPER_H
