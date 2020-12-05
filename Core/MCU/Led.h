//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef HABSAT_OBC_LED_H
#define HABSAT_OBC_LED_H

#include "GPIOPin.h"

namespace hw {

class Led : public Noncopyable {
 public:
  explicit Led(GPIOPin* pin);

  /**
   * Turns the LED on.
   */
  void on();

  /**
   * Turns the LED off.
   */
  void off();

  /**
   * Toggles the LED.
   */
  void toggle();

  /**
   * Checks whether the LED is on.
   * @return
   */
  [[nodiscard]] auto isOn() const -> bool;

 private:
  GPIOPin* const pin_;
};

}  // namespace hw

#endif  // HABSAT_OBC_LED_H
