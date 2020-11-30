//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef RCC_SYS_HARDWARE_H
#define RCC_SYS_HARDWARE_H

#include "Button.h"
#include "I2CBus.h"
#include "Lcd.h"
#include "Led.h"
#include "Rtc.h"
#include "Sensor.h"
#include "constants.h"
#include "handles.h"

/**
 * Holds all instances of the classes that represent peripherals
 */
struct Hardware
{
    constexpr static hw::I2CBus i2c{hw::Handles::i2c3};
    constexpr static hw::SPIBus spi{hw::Handles::spi2};

    hw::GPIOPin pinC13{GPIOC, GPIO_PIN_13};
    hw::GPIOPin pinA5{GPIOA, GPIO_PIN_5};

    hw::Button button{&pinC13};
    hw::Led led{&pinA5};
    hw::Lcd lcd{4, 20, &i2c, constants::lcdSlaveAddress};
    hw::Rtc rtc{&i2c, constants::rtcSlaveAddress};
    hw::Sensor sensor{&spi};
public:
    void init();
};

#endif//RCC_SYS_HARDWARE_H
