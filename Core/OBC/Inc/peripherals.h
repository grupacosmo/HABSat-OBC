//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_PERIPHERALS_H
#define RCC_SYS_PERIPHERALS_H

#include "handles.h"
#include "constants.h"
#include "wifi.h"
#include "lcd.h"
#include "rtc.h"
#include "sensor.h"

/**
 * Holds all instances of the classes that represent peripherals
 */
struct Peripherals
{
    Lcd lcd{4, 20, &hi2c3, constants::LCD_SLAVE_ADDRESS};
    WiFi wifi{&huart1};
    Rtc rtc{&hi2c3, constants::RTC_SLAVE_ADDRESS};
    Sensor sensor{&hspi2};

public:
    void init();
};

#endif//RCC_SYS_PERIPHERALS_H
