//
// Created by vixu on 4/18/21.
//

#ifndef HABSAT_OBC_TO_BYTES_HPP
#define HABSAT_OBC_TO_BYTES_HPP

#include <cstdint>

namespace habsat::utils {

template <typename T>
constexpr auto to_bytes(const T* data) -> const std::uint8_t* {
    return reinterpret_cast<const std::uint8_t*>(data);
}

template <typename T>
constexpr auto to_bytes(T* data) -> std::uint8_t* {
    return reinterpret_cast<std::uint8_t*>(data);
}

}  // namespace habsat::utils

#endif  // HABSAT_OBC_TO_BYTES_HPP
