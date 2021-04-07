//
// Created by Wiktor Wieclaw on 8/20/20.
//

#ifndef HABSAT_OBC_MCU_BOARD_BUTTON_HPP
#define HABSAT_OBC_MCU_BOARD_BUTTON_HPP

#include "MCU_Board/gpio_pin.hpp"

namespace habsat::mcuBoard {

class Button {
   public:
    explicit Button(GPIOPin* pin) : pin_(pin) {}

    /**
     * Checks whether the button is pressed
     * @return
     */
    [[nodiscard]] auto isPressed() const -> bool { return pin_->read() == GPIOState::Reset; }

   private:
    GPIOPin* const pin_;
};

}  // namespace habsat::mcuBoard

#endif  // HABSAT_OBC_MCU_BOARD_BUTTON_HPP
