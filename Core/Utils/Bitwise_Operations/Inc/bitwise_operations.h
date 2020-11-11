//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef RCC_SYS_BITWISE_OPERATIONS_H
#define RCC_SYS_BITWISE_OPERATIONS_H

#include <cstddef>
#include <cstdint>

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

    /**
     * Sets variable number of bits.
     * @tparam Integral
     * @param byte
     * @param bit from 0 to 7
     * @return
     */
    template<typename... Integral>
    constexpr uint8_t setBits(uint8_t byte, Integral... bit)
    {
        return ((byte | (1 << bit)), ...);
    }

    /**
     * Clears variable number of bits.
     * @tparam Integral
     * @param byte
     * @param bit from 0 to 7
     * @return
     */
    template<typename... Integral>
    constexpr uint8_t clearBits(uint8_t byte, Integral... bit)
    {
        return ((byte & ~(1 << bit)), ...);
    }

    constexpr uint16_t swapBytes(uint16_t twoBytes)
    {
        return (twoBytes >> 8) | (twoBytes << 8);
    }
}


#endif //RCC_SYS_BITWISE_OPERATIONS_H
