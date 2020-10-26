//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_OS_QUEUE_H
#define RCC_SYS_OS_QUEUE_H

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
        Queue();

        /**
         * Initializes the Queue.
         */
        void init();

        /**
         * Sends object into the Queue
         * @param object
         * @param ticks_to_wait
         */
        void send(Type &object, TickType_t ticks_to_wait);

        /**
         * Receives object from the Queue
         * @param object_holder
         * @param ticks_to_wait
         * @return
         */
        bool receive(Type &object_holder, TickType_t ticks_to_wait);
    private:
        QueueHandle_t m_queue_handle;
    };

    template<typename Type, size_t size>
    Queue<Type, size>::Queue()
            : m_queue_handle(NULL)
    {

    }
    template<typename Type, size_t size>
    void Queue<Type, size>::init()
    {
        m_queue_handle = xQueueCreate(size, sizeof(Type));
    }

    template<typename Type, size_t size>
    void Queue<Type, size>::send(Type &object, TickType_t ticks_to_wait)
    {
        xQueueSend(m_queue_handle, &object, ticks_to_wait);
    }

    template<typename Type, size_t size>
    bool Queue<Type, size>::receive(Type &object_holder, TickType_t ticks_to_wait)
    {
        return xQueueReceive(m_queue_handle, &object_holder, ticks_to_wait);
    }

}

#endif//RCC_SYS_OS_QUEUE_H
