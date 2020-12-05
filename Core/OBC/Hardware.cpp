//
// Created by Wiktor Wieclaw on 9/7/20.
//
#include "Hardware.h"

#include "hardware_config.h"

void Hardware::init() {
#if HW_LCD == 1
  lcd.init();
#endif
#if HW_RTC == 1
  rtc.init();
#endif
#if HW_SENSOR == 1
  sensor.init(hw::Sensor::ConfigFlags::Temperature16Bit,
              hw::Sensor::ConfigFlags::PressureUltraLowPower,
              hw::Sensor::ConfigFlags::HumidityStandard, hw::Sensor::ConfigFlags::NormalMode);
  sensor.configure(hw::Sensor::ConfigFlags::Standby10Ms, hw::Sensor::ConfigFlags::FilterOff);
#endif
}
