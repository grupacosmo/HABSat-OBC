//
// Created by Wiktor Wieclaw on 12/1/20.
//

#ifndef RCC_SYS_NONCOPYABLE_H
#define RCC_SYS_NONCOPYABLE_H

class Noncopyable {
 protected:
  Noncopyable()  = default;
  ~Noncopyable() = default;

 public:
  Noncopyable(const Noncopyable&) = delete;
  auto operator=(const Noncopyable&) -> Noncopyable& = delete;
};

#endif  // RCC_SYS_NONCOPYABLE_H
