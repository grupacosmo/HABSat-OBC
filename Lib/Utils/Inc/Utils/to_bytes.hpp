//
// Created by vixu on 4/18/21.
//

#ifndef HABSAT_OBC_TO_BYTES_HPP
#define HABSAT_OBC_TO_BYTES_HPP

#include <cstdint>

namespace habsat::utils {

inline auto to_bytes(const char* data) -> const std::uint8_t* {
    return reinterpret_cast<const std::uint8_t*>(data);
}

inline auto to_bytes(char* data) -> std::uint8_t* {
    return reinterpret_cast<std::uint8_t*>(data);
}

}  // namespace habsat::utils

#endif  // HABSAT_OBC_TO_BYTES_HPP
