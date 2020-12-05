//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_BUS_H
#define RCC_SYS_BUS_H

#include "handles.h"
#include "Noncopyable.h"

namespace hw
{

enum class BusResult
{
    Ok = HAL_OK,
    Error = HAL_ERROR,
    Busy = HAL_BUSY,
    Timeout = HAL_TIMEOUT
};

template<typename T>
class Bus : public Noncopyable
{
protected:
    T *const handle_;
    constexpr explicit Bus(T *handle);

public:
    constexpr T *getHandle() const;
};

template<typename T>
constexpr Bus<T>::Bus(T *handle) : handle_(handle)
{
}

template<typename T>
constexpr T *Bus<T>::getHandle() const
{
    return handle_;
}

}
#endif//RCC_SYS_BUS_H
