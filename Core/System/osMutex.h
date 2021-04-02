//
// Created by Wiktor Wieclaw on 12/20/20.
//

#ifndef HABSAT_OBC_OSMUTEX_H
#define HABSAT_OBC_OSMUTEX_H

#include "FreeRTOS.h"
#include "semphr.h"

namespace os {

class Mutex {
   public:
    Mutex() : handle_(xSemaphoreCreateMutex()) {}

    auto take(uint32_t timeout) -> bool { return xSemaphoreTake(handle_, timeout); }

    auto give() -> bool { return xSemaphoreGive(handle_); }

   private:
    SemaphoreHandle_t handle_;
};

class LockMutex {
   public:
    explicit LockMutex(Mutex& mutex, uint32_t timeout) : mutex_(mutex) {
        mutex_.take(timeout);
    }

    ~LockMutex() { mutex_.give(); }

    auto get() -> Mutex& { return mutex_; }

   private:
    Mutex& mutex_;
};

}  // namespace os

#endif  // HABSAT_OBC_OSMUTEX_H
