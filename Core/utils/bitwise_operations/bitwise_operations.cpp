//
// Created by vixu on 8/7/20.
//

#include "bitwise_operations.h"

namespace bitwise
{
    std::byte nibble_low(std::byte b)
    {
        return (b<<4) & std::byte{0xf0};
    }


    std::byte nibble_high(std::byte b)
    {
        return b & std::byte{0xf0};
    }
}