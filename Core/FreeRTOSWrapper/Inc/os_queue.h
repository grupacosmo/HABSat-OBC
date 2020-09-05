//
// Created by Wiktor Wieclaw on 9/5/20.
//

#ifndef RCC_SYS_OS_QUEUE_H
#define RCC_SYS_OS_QUEUE_H

#include "FreeRTOS.h"
#include "queue.h"
#include <array>

namespace os
{
    template<typename Type, size_t size>
    class Queue
    {
    public:
        Queue();
        void init();
        void send(Type &object, TickType_t ticks_to_wait);
        Type receive(TickType_t ticks_to_wait);
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
    Type Queue<Type, size>::receive(TickType_t ticks_to_wait)
    {
        Type received_object;
        xQueueReceive(m_queue_handle, &received_object, ticks_to_wait);
        return received_object;
    }

}

#endif//RCC_SYS_OS_QUEUE_H
