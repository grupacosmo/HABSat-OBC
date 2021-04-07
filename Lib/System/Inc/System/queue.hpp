//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef HABSAT_OBC_SYSTEM_QUEUE_HPP
#define HABSAT_OBC_SYSTEM_QUEUE_HPP

#include <FreeRTOS.h>
#include <queue.h>

namespace habsat::system {

template <typename Type>
class Queue {
   public:
    /**
     * Constructs the object.
     */
    explicit Queue(size_t size) : queueHandle_(xQueueCreate(size, sizeof(Type))) {}

    /**
     * Sends object into the Queue
     * @param object
     * @param ticksToWait
     */
    void send(Type& object, TickType_t ticksToWait) {
        xQueueSend(queueHandle_, &object, ticksToWait);
    }

    /**
     * Receives object from the Queue
     * @param objectHolder
     * @param ticksToWait
     * @return
     */
    auto receive(Type& objectHolder, TickType_t ticksToWait) -> bool {
        return xQueueReceive(queueHandle_, &objectHolder, ticksToWait);
    }

   private:
    QueueHandle_t queueHandle_;
};

}  // namespace habsat::system

#endif  // HABSAT_OBC_SYSTEM_QUEUE_HPP
