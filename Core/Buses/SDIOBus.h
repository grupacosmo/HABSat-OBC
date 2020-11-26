//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef RCC_SYS_SDIOBUS_H
#define RCC_SYS_SDIOBUS_H

#include "Bus.h"

namespace hw
{

class SDIOBus : public Bus<SDIOHandle>
{
public:
    constexpr explicit SDIOBus(SDIOHandle* handle) : Bus(handle) { }
private:
    static constexpr uint32_t defaultTimeout_ = 100;
};

}
#endif//RCC_SYS_SDIOBUS_H
