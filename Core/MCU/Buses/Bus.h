//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_BUS_H
#define RCC_SYS_BUS_H

#include "Noncopyable.h"
#include "handles.h"

namespace hw {

enum class BusResult { Ok, Error, Busy, Timeout };

template <typename T>
class Bus : public Noncopyable {
 protected:
  T* const handle_;
  constexpr explicit Bus(T* handle);

 public:
  constexpr auto getHandle() const -> T*;
};

template <typename T>
constexpr Bus<T>::Bus(T* handle) : handle_(handle) {}

template <typename T>
constexpr auto Bus<T>::getHandle() const -> T* {
  return handle_;
}

}  // namespace hw
#endif  // RCC_SYS_BUS_H
