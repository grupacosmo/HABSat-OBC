//
// Created by Wiktor Wieclaw on 8/7/20.
//

#ifndef HABSAT_OBC_MCU_BOARD_LED_HPP
#define HABSAT_OBC_MCU_BOARD_LED_HPP

#include "MCU_Board/gpio_pin.hpp"

namespace habsat::mcuBoard {

class Led{
   public:
    explicit Led(GPIOPin& pin) : pin_(pin) {}

    /**
     * Turns the LED on.
     */
    void on() { pin_.set(); }

    /**
     * Turns the LED off.
     */
    void off() { pin_.reset(); }

    /**
     * Toggles the LED.
     */
    void toggle() { pin_.toggle(); }

    /**
     * Checks whether the LED is on.
     * @return
     */
    [[nodiscard]] auto isOn() const -> bool { return pin_.read() == GPIOState::Set; }

   private:
    GPIOPin& pin_;
};

}  // namespace habsat::mcuBoard

#endif  // HABSAT_OBC_MCU_BOARD_LED_HPP
