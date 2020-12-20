//
// Created by Wiktor Wieclaw on 12/20/20.
//

#ifndef HABSAT_OBC_OSMUTEX_H
#define HABSAT_OBC_OSMUTEX_H

#include "FreeRTOS.h"
#include "semphr.h"

namespace os {

class LockMutex;

class Mutex {
 public:
  Mutex();
  auto take(uint32_t timeout = portMAX_DELAY) -> bool;
  auto give() -> bool;

 private:
  SemaphoreHandle_t handle_;
};

class LockMutex {
 public:
  explicit LockMutex(Mutex& mutex, uint32_t timeout = portMAX_DELAY);
  ~LockMutex();
  auto get() -> Mutex&;

 private:
  Mutex& mutex_;
};

inline Mutex::Mutex() : handle_(xSemaphoreCreateMutex()) {}

inline auto Mutex::take(uint32_t timeout) -> bool { return xSemaphoreTake(handle_, timeout); };

inline auto Mutex::give() -> bool { return xSemaphoreGive(handle_); }

inline LockMutex::LockMutex(Mutex& mutex, uint32_t timeout) : mutex_(mutex) {
  mutex_.take(timeout);
}

inline LockMutex::~LockMutex() { mutex_.give(); }

inline auto LockMutex::get() -> Mutex& { return mutex_; }

}  // namespace os

#endif  // HABSAT_OBC_OSMUTEX_H
