//
// Created by Wiktor Wieclaw on 11/16/20.
//

#ifndef HABSAT_OBC_BUSES_RESULT_HPP
#define HABSAT_OBC_BUSES_RESULT_HPP

namespace habsat::buses {

enum class Result { Ok, Error, Busy, Timeout };

namespace detail {
    [[nodiscard]] inline auto toResult(HAL_StatusTypeDef status) -> Result {
        return static_cast<Result>(status);
    }
}

}  // namespace habsat::buses

#endif  // HABSAT_OBC_BUSES_RESULT_HPP
