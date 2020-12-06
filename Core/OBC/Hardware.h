//
// Created by Wiktor Wieclaw on 9/7/20.
//

#ifndef HABSAT_OBC_HARDWARE_H
#define HABSAT_OBC_HARDWARE_H

#include "Button.h"
#include "I2CBus.h"
#include "SPIBus.h"
#include "Lcd.h"
#include "Led.h"
#include "Rtc.h"
#include "Sensor.h"
#include "SDReader.h"
#include "constants.h"
#include "handles.h"

/**
 * Holds all instances of the classes that represent peripherals
 */
struct Hardware : public Noncopyable {
  constexpr static hw::I2CBus i2c{hw::Handles::i2c3};
  constexpr static hw::SPIBus spi{hw::Handles::spi2};

  hw::GPIOPin pinC13{GPIOC, GPIO_PIN_13};
  hw::GPIOPin pinA5{GPIOA, GPIO_PIN_5};
  hw::ChipSelect sensorCS{GPIOC, GPIO_PIN_3};

  hw::Button button{&pinC13};
  hw::Led led{&pinA5};
  hw::Lcd lcd{4, 20, &i2c, constants::lcdSlaveAddress};
  hw::Rtc rtc{&i2c, constants::rtcSlaveAddress};
  hw::Sensor sensor{spi, sensorCS};
  hw::SDReader sdReader; // TODO work it out

 public:
  void init();
};

#endif  // HABSAT_OBC_HARDWARE_H
