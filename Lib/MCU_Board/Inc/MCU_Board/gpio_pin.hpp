//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_MCU_BOARD_GPIO_PIN_HPP
#define HABSAT_OBC_MCU_BOARD_GPIO_PIN_HPP

#include <stm32f4xx.h>

#include "Utils/noncopyable.hpp"

namespace habsat::mcuBoard {

enum GPIOState { Reset, Set };

class GPIOPin : public utils::Noncopyable {
   public:
    GPIOPin(GPIO_TypeDef* pinType, uint16_t pinNumber) : pinType_(pinType), pinNumber_(pinNumber) {}

    void set() { HAL_GPIO_WritePin(pinType_, pinNumber_, GPIO_PIN_SET); }

    void reset() { HAL_GPIO_WritePin(pinType_, pinNumber_, GPIO_PIN_RESET); }

    void toggle() { HAL_GPIO_TogglePin(pinType_, pinNumber_); }

    [[nodiscard]] auto read() const -> GPIOState {
        return static_cast<GPIOState>(HAL_GPIO_ReadPin(pinType_, pinNumber_));
    }

   private:
    GPIO_TypeDef* const pinType_;
    const uint16_t pinNumber_;
};

}  // namespace habsat::mcuBoard

#endif  // HABSAT_OBC_MCU_BOARD_GPIO_PIN_HPP
