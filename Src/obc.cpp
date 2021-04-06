//
// Created by Wiktor Wieclaw on 10/1/20.
//
#include "obc.hpp"

using habsat::system::Priority;

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
      inputTask{"input", 128, system::Priority::Interrupt, tasks::blink::inputTaskFn},
      blinkTask{"blink", 128, system::Priority::Idle, tasks::blink::blinkTaskFn},
      displayTask{"display", 256, system::Priority::Idle, tasks::display::taskFn},
      measureTimeTask{"measure_time", 128, system::Priority::Idle, tasks::measureTime::taskFn},
      measureWeatherTask{
            "measure_weather",
            256,
            system::Priority::Idle,
            tasks::measureWeather::taskFn},
      sdSaveTask{"sd_save", 1024, system::Priority::Idle, tasks::sdSave::taskFn} {}

void habsat::Obc::init() {
#if HW_LCD == 1
    lcd.init();
#endif
#if HW_RTC == 1
    rtc.init();
#endif
#if HW_SENSOR == 1
    sensor.init(
          sensor::ConfigFlags::Temperature16Bit,
          sensor::ConfigFlags::PressureUltraLowPower,
          sensor::ConfigFlags::HumidityStandard,
          sensor::ConfigFlags::NormalMode);
    sensor.configure(
          sensor::ConfigFlags::Standby10Ms,
          sensor::ConfigFlags::FilterOff);
#endif

    inputTask.addToScheduler();
    blinkTask.addToScheduler();
    displayTask.addToScheduler();
    measureTimeTask.addToScheduler();
    measureWeatherTask.addToScheduler();
    sdSaveTask.addToScheduler();
}

auto habsat::getObc() -> Obc& {
    static Obc obc;
    return obc;
}