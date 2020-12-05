//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include "Button.h"

namespace hw {

Button::Button(GPIOPin* pin) : pin_(pin) {}

auto Button::isPressed() const -> bool { return pin_->read() == GPIOState::Reset; }

}  // namespace hw