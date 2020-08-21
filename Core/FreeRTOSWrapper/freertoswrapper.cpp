//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include "freertoswrapper.h"

namespace os
{
    namespace details
    {
        static void call_task_function(void *args)
        {
            auto &f = *static_cast<std::function<void()> *>(args);
            f();
        }
    }

    void create_task(std::function<void()> &task_code,
                     const char * const name,
                     const configSTACK_DEPTH_TYPE usStackDepth,
                     UBaseType_t uxPriority,
                     TaskHandle_t * const pxCreatedTask )
    {
        xTaskCreate(details::call_task_function, name, usStackDepth, static_cast<void *>(&task_code), uxPriority,
                    pxCreatedTask);
    }

}