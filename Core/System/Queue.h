//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_QUEUE_H
#define RCC_SYS_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"

namespace os
{
    template<typename Type, size_t size>
    class Queue
    {
    public:
        /**
         * Constructs the object.
         */
        constexpr Queue();

        /**
         * Sends object into the Queue
         * @param object
         * @param ticksToWait
         */
        constexpr void send(Type &object, TickType_t ticksToWait);

        /**
         * Receives object from the Queue
         * @param objectHolder
         * @param ticksToWait
         * @return
         */
        inline bool receive(Type &objectHolder, TickType_t ticksToWait);
    private:
        const QueueHandle_t queueHandle_;
    };

    template<typename Type, size_t size>
    constexpr Queue<Type, size>::Queue()
    : queueHandle_(xQueueCreate(size, sizeof(Type)))
    {

    }

    template<typename Type, size_t size>
    constexpr void Queue<Type, size>::send(Type &object, TickType_t ticksToWait)
    {
        xQueueSend(queueHandle_, &object, ticksToWait);
    }

    template<typename Type, size_t size>
    bool Queue<Type, size>::receive(Type &objectHolder, TickType_t ticksToWait)
    {
        return xQueueReceive(queueHandle_, &objectHolder, ticksToWait);
    }

}

#endif//RCC_SYS_QUEUE_H
