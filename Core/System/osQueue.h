//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef HABSAT_OBC_OSQUEUE_H
#define HABSAT_OBC_OSQUEUE_H

#include "FreeRTOS.h"
#include "queue.h"

namespace os {

template <typename Type>
class Queue : public Noncopyable {
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

}  // namespace os

#endif  // HABSAT_OBC_OSQUEUE_H
