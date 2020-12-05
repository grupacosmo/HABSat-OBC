//
// Created by Wiktor Wieclaw on 12/1/20.
//

#ifndef HABSAT_OBC_NONCOPYABLE_H
#define HABSAT_OBC_NONCOPYABLE_H

class Noncopyable {
 protected:
  Noncopyable()  = default;
  ~Noncopyable() = default;

 public:
  Noncopyable(const Noncopyable&) = delete;
  auto operator=(const Noncopyable&) -> Noncopyable& = delete;
};

#endif  // HABSAT_OBC_NONCOPYABLE_H
