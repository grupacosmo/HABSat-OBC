//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef HABSAT_OBC_OSTASK_H
#define HABSAT_OBC_OSTASK_H

#include "FreeRTOS.h"
#include "Noncopyable.h"
#include "task.h"

namespace os {

/**
 * Priority of a task.
 */
enum class Priority {
    Idle      = tskIDLE_PRIORITY,
    Interrupt = configMAX_SYSCALL_INTERRUPT_PRIORITY,
};

enum class TaskState { Running, Ready, Blocked, Suspended, Deleted, Invalid };

class Task : public Noncopyable {
   public:
    using StackDepth = configSTACK_DEPTH_TYPE;

    /**
     * Task's constructor.
     * @param name          Name of the task.
     * @param stackDepth  Approximate size of the stack needed for the task's function.
     * @param priority      Priority of the task.
     * @param taskCode     Pointer to a function that contains the code of the task.
     * @param args          void* used to pass arguments to the task
     */
    Task(const char* const name,
         const StackDepth stackDepth,
         const Priority& priority,
         void (*taskCode)(void*))
        : taskHandle_(nullptr),
          taskCode_(taskCode),
          name_(name),
          stackDepth_(stackDepth),
          priority_(priority) {}

    /**
     * Suspends the task.
     */
    void suspend() const { vTaskSuspend(taskHandle_); }

    /**
     * Resumes the task.
     */
    void resume() const { vTaskResume(taskHandle_); }

    /**
     * Deletes the task form scheduler.
     */
    void detach() const { vTaskDelete(taskHandle_); }

    /**
     *
     * @return State from os::TaskState enum.
     */
    [[nodiscard]] auto getState() const -> TaskState {
        return static_cast<TaskState>(eTaskGetState(taskHandle_));
    }

    /**
     * Resumes the task from the interrupt handler.
     */
    void resumeFromISR() const { portYIELD_FROM_ISR(xTaskResumeFromISR(taskHandle_)); }

    /**
     * Allocates memory and adds the task to the scheduler.
     */
    void addToScheduler(void* params) const {
        xTaskCreate(
              taskCode_,
              name_,
              stackDepth_,
              const_cast<void*>(params),
              static_cast<int>(priority_),
              const_cast<TaskHandle_t*>(&taskHandle_));
    }

   private:
    TaskHandle_t taskHandle_;
    void (*const taskCode_)(void*);
    const char* const name_;
    const StackDepth stackDepth_;
    const Priority priority_;
};

/**
 * Starts the scheduler. In other words, starts all the tasks.
 */
inline void startScheduler() { vTaskStartScheduler(); }

}  // namespace os

namespace os::thisTask {

/**
 * Allows the task to resume itself.
 */
inline void suspend() { vTaskSuspend(nullptr); }

/**
 * Allows the task to suspend itself.
 */
inline void resume() { vTaskResume(nullptr); }

/**
 * Allows the task to delete itself from scheduler.
 */
inline void detach() { vTaskDelete(nullptr); }

/**
 * Stops the task for no less than the time specified in milliseconds. Note that the delay
 * may last a little bit longer.
 * @param milliseconds
 */
inline void delay(uint32_t milliseconds) { vTaskDelay(pdMS_TO_TICKS(milliseconds)); }

/**
 * Stops the task for the exact time specified in milliseconds.
 *
 * @param previousWakeTime
 * @param milliseconds
 */
inline void delayUntil(TickType_t* previousWakeTime, uint32_t milliseconds) {
    vTaskDelayUntil(previousWakeTime, pdMS_TO_TICKS(milliseconds));
}

}  // namespace os::thisTask

#endif  // HABSAT_OBC_OSTASK_H