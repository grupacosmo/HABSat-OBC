//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "obc.hpp"

habsat::Obc::Obc()
    : i2c{hi2c3},
      spi{hspi2},
      uart(huart1),
      pinC13{GPIOC, GPIO_PIN_13},
      pinA5{GPIOA, GPIO_PIN_5},
      sensorCS{GPIOC, GPIO_PIN_3},
      button{&pinC13},
      led{pinA5},
      lcd{4, 20, i2c, constants::lcdSlaveAddress},
      rtc{i2c, constants::rtcSlaveAddress},
      sensor{spi, sensorCS},
      inputTask{"input", 128, system::Priority::Interrupt, blink::inputTaskFn},
      blinkTask{"blink", 128, system::Priority::Idle, blink::blinkTaskFn},
      displayTask{"display", 256, system::Priority::Idle, display::taskFn},
      measureTimeTask{"measure_time", 128, system::Priority::Idle, measureTime::taskFn},
      measureWeatherTask{"measure_weather", 256, system::Priority::Idle, measureWeather::taskFn},
      sdSaveTask{"sd_save", 1024, system::Priority::Idle, sdSave::taskFn} {}

void habsat::Obc::init() {
#if HW_LCD == 1
    lcd.init();
#endif
#if HW_RTC == 1
    rtc.init();
#endif
#if HW_SENSOR == 1
    sensor.init(
          sensor::Sensor::ConfigFlags::Temperature16Bit,
          sensor::Sensor::ConfigFlags::PressureUltraLowPower,
          sensor::Sensor::ConfigFlags::HumidityStandard,
          sensor::Sensor::ConfigFlags::NormalMode);
    sensor.configure(
          sensor::Sensor::ConfigFlags::Standby10Ms,
          sensor::Sensor::ConfigFlags::FilterOff);
#endif

    void* obcParam = static_cast<void*>(this);

    inputTask.addToScheduler(obcParam);
    blinkTask.addToScheduler(obcParam);
    displayTask.addToScheduler(obcParam);
    measureTimeTask.addToScheduler(obcParam);
    measureWeatherTask.addToScheduler(obcParam);
    sdSaveTask.addToScheduler(obcParam);
}

auto habsat::obc() -> Obc& {
    static Obc obc;
    return obc;
}