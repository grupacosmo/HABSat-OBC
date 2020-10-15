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
        Queue();
        void initalize();
        void send(Type &object, TickType_t ticks_to_wait);
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
    void Queue<Type, size>::initalize()
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
