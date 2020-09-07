//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_PERIPHERALS_H
#define RCC_SYS_PERIPHERALS_H

#include "handles.h"
#include "constants.h"
#include "wifi.h"
#include "lcd.h"
#include "led.h"

namespace global
{
    struct Peripherals {
        Led led;
        Lcd lcd{4, 20, &hi2c1, constants::LCD_SLAVE_ADDRESS};
        WiFi wifi{huart1};

        void initialize();
    };
}


#endif//RCC_SYS_PERIPHERALS_H
