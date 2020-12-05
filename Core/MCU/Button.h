//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef HABSAT_OBC_BUTTON_H
#define HABSAT_OBC_BUTTON_H

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

#endif  // HABSAT_OBC_BUTTON_H
