//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_HARDWARE_H
#define RCC_SYS_HARDWARE_H

#include "lcd.h"
#include "rtc.h"
#include "sensor.h"
#include "wifi.h"
#include "I2CBus.h"
#include "constants.h"
#include "handles.h"
#include "button.h"
#include "led.h"


/**
 * Holds all instances of the classes that represent peripherals
 */
struct Hardware {
    hw::I2CBus i2c{hw::Handles::i2c3};
    hw::SPIBus spi{hw::Handles::spi2};

    hw::Button button;
    hw::Led led;
    hw::Lcd lcd{4, 20, &i2c, constants::lcdSlaveAddress};
    hw::WiFi wifi{hw::Handles::uart1};
    hw::Rtc rtc{&i2c, constants::rtcSlaveAddress};
    hw::Sensor sensor{&spi};
public:
    void init();
};

#endif//RCC_SYS_HARDWARE_H
