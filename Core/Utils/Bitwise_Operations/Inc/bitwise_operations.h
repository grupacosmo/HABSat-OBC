//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef RCC_SYS_BITWISE_OPERATIONS_H
#define RCC_SYS_BITWISE_OPERATIONS_H

#include <cstddef>

namespace bitwise
{
    /**
     * Returns the lower nibble (4 bits) of a byte.
     * @param b
     * @return
     */
    std::byte nibble_low(std::byte b);

    /**
     * Returns the higher nibble (4 bits) of a byte.
     * @param b
     * @return
     */
    std::byte nibble_high(std::byte b);
}


#endif //RCC_SYS_BITWISE_OPERATIONS_H
