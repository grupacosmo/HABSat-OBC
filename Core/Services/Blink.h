//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_BLINK_H
#define RCC_SYS_BLINK_H

#include "Button.h"
#include "Led.h"
#include "osTask.h"

namespace services {

class Blink : public Noncopyable {
 public:
  explicit Blink(hw::Led* led);
  void init();

 public:
  static void inputInterruptHandler();

 private:
  [[noreturn]] static void inputTaskFunction(void* args);
  [[noreturn]] static void blinkTaskFunction(void* args);

 private:
  static os::Task inputTask_;
  static os::Task blinkTask_;

 private:
  hw::Led* led_;
};

}  // namespace services
#endif  // RCC_SYS_BLINK_H
