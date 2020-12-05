//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef RCC_SYS_BITWISEOPERATIONS_H
#define RCC_SYS_BITWISEOPERATIONS_H

#include <cstddef>
#include <cstdint>

namespace bitwise {
/**
 * Returns the lower nibble (4 bits) of a byte.
 * @param byte
 * @return
 */
[[nodiscard]] constexpr auto nibbleLow(uint8_t byte) -> uint8_t { return byte & 0x0f; }

/**
 * Returns the higher nibble (4 bits) of a byte.
 * @param byte
 * @return
 */
[[nodiscard]] constexpr auto nibbleHigh(uint8_t byte) -> uint8_t { return byte & 0xf0; }

/**
 * Sets variable number of bits.
 * @tparam bits
 * @param byte
 * @return
 */
template <size_t... bits>
[[nodiscard]] constexpr auto setBits(uint8_t byte) -> uint8_t {
  return ((byte |= (1 << bits)), ...);
}

/**
 * Clears variable number of bits.
 * @tparam bits
 * @param byte
 * @return
 */
template <size_t... bits>
[[nodiscard]] constexpr auto clearBits(uint8_t byte) -> uint8_t {
  return ((byte &= ~(1 << bits)), ...);
}

/**
 * Swaps bytes in a uint16_t
 * @param twoBytes
 * @return
 */
[[nodiscard]] constexpr auto swapBytes(uint16_t twoBytes) -> uint16_t {
  return (twoBytes >> 8) | (twoBytes << 8);
}

[[nodiscard]] constexpr auto concat2Bytes(uint8_t byte1, uint8_t byte2) -> int16_t {
  return (byte1 << 8) | (byte2);
}

[[nodiscard]] constexpr auto concat3Bytes(uint8_t byte1, uint8_t byte2, uint8_t byte3) -> int32_t {
  return (byte1 << 16) | (byte2 << 8) | (byte3);
}

}  // namespace bitwise

#endif  // RCC_SYS_BITWISEOPERATIONS_H
