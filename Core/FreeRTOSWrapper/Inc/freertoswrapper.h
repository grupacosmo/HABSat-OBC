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
    namespace details
    {
        static void task(void *args)
        {
            auto f = *static_cast<std::function<void()> *>(args);
            f();
        }
    }

    void create_task(std::function<void()> &task_code,
                     const char * const name,
                     const configSTACK_DEPTH_TYPE usStackDepth,
                     UBaseType_t uxPriority,
                     TaskHandle_t * const pxCreatedTask )
    {
        xTaskCreate(details::task, name, usStackDepth, static_cast<void*>(&task_code), uxPriority, pxCreatedTask);
    }
};

#endif //RCC_SYS_FREERTOSWRAPPER_H
