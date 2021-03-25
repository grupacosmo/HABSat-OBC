//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "Obc.h"

Obc::Obc()
    : i2c{&hi2c3},
      spi{&hspi2},
      pinC13{GPIOC, GPIO_PIN_13},
      pinA5{GPIOA, GPIO_PIN_5},
      sensorCS{GPIOC, GPIO_PIN_3},
      button{&pinC13},
      led{&pinA5},
      lcd{4, 20, &i2c, constants::lcdSlaveAddress},
      rtc{&i2c, constants::rtcSlaveAddress},
      sensor{spi, sensorCS},
      blink{&led},
      measureTime{&rtc},
      measureWeather{&sensor},
      display{&lcd, &measureWeather.getBuffer(), &measureTime.getBuffer()} {}

void Obc::init() {
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
  blink.init();
  display.init();
  measureTime.init();
  measureWeather.init();
}