//
// Created by Wiktor Wieclaw on 8/20/20.
//

#include <button.h>

bool Button::is_pressed() const
{
    return HAL_GPIO_ReadPin(TYPE, PIN) == GPIO_PIN_RESET;
}

