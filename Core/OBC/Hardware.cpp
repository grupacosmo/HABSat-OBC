//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "Hardware.h"

#include "hardware_config.h"

Hardware::Hardware()
    : i2c{&hi2c3},
      spi{&hspi2},
      pinC13{GPIOC, GPIO_PIN_13},
      pinA5{GPIOA, GPIO_PIN_5},
      sensorCS{GPIOC, GPIO_PIN_3},
      button{&pinC13},
      led{&pinA5},
      lcd{4, 20, &i2c, constants::lcdSlaveAddress},
      rtc{&i2c, constants::rtcSlaveAddress},
      sensor{spi, sensorCS} {}

void Hardware::init() {
#if HW_LCD == 1
  lcd.init();
#endif
#if HW_RTC == 1
  rtc.init();
#endif
#if HW_SENSOR == 1
  sensor.init(sensor::Sensor::ConfigFlags::Temperature16Bit,
              sensor::Sensor::ConfigFlags::PressureUltraLowPower,
              sensor::Sensor::ConfigFlags::HumidityStandard,
              sensor::Sensor::ConfigFlags::NormalMode);
  sensor.configure(sensor::Sensor::ConfigFlags::Standby10Ms,
                   sensor::Sensor::ConfigFlags::FilterOff);
#endif
}
