//
// Created by vixu on 4/7/21.
//

#ifndef HABSAT_OBC_TO_UNDERLYING_HPP
#define HABSAT_OBC_TO_UNDERLYING_HPP

#include <type_traits>

namespace habsat::utils {

template <typename Enum, typename = typename std::enable_if_t<std::is_enum_v<Enum>, Enum>>
constexpr auto toUnderlying(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

}  // namespace habsat::utils

#endif  // HABSAT_OBC_TO_UNDERLYING_HPP
