//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_BUS_H
#define RCC_SYS_BUS_H

#include "handles.h"

namespace hw
{

enum class BusResult
{
    ok = HAL_OK,
    error = HAL_ERROR,
    busy = HAL_BUSY,
    timeout = HAL_TIMEOUT
};

template<typename T>
class Bus
{
protected:
    T *const handle;
    explicit Bus(T *handle);

public:
    T *getHandle() const;
};

template<typename T>
Bus<T>::Bus(T *handle) : handle(handle)
{
}

template<typename T>
T *Bus<T>::getHandle() const
{
    return handle;
}

}
#endif//RCC_SYS_BUS_H
