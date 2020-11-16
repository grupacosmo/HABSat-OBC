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
    explicit SDIOBus(SDIOHandle* handle);
private:
    static constexpr uint32_t defaultTimeout = 100;
};

}
#endif//RCC_SYS_SDIOBUS_H
