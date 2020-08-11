//
// Created by Wiktor Wieclaw on 8/7/20.
//

#include "bitwise_operations.h"

namespace bitwise
{
    std::byte nibble_low(std::byte b)
    {
        return b & std::byte{0x0f};
    }


    std::byte nibble_high(std::byte b)
    {
        return b & std::byte{0xf0};
    }
}