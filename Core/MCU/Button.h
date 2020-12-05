//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef RCC_SYS_BUTTON_H
#define RCC_SYS_BUTTON_H

#include "GPIOPin.h"

namespace hw {

class Button : public Noncopyable {
 public:
  explicit Button(GPIOPin* pin);

  /**
   * Checks whether the button is pressed
   * @return
   */
  [[nodiscard]] auto isPressed() const -> bool;

 private:
  GPIOPin* const pin_;
};

}  // namespace hw

#endif  // RCC_SYS_BUTTON_H
