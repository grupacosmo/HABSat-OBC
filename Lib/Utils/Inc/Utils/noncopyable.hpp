//
// Created by Wiktor Wieclaw on 12/1/20.
//

#ifndef HABSAT_OBC_UTILS_NONCOPYABLE_HPP
#define HABSAT_OBC_UTILS_NONCOPYABLE_HPP

namespace habsat::utils {

class Noncopyable {
   protected:
    Noncopyable()  = default;
    ~Noncopyable() = default;

   public:
    Noncopyable(const Noncopyable&) = delete;
    auto operator=(const Noncopyable&) -> Noncopyable& = delete;
};

}  // namespace habsat::utils

#endif  // HABSAT_OBC_NONCOPYABLE_HPP
