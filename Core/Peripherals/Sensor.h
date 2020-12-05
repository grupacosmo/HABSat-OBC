//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef HABSAT_OBC_SENSOR_H
#define HABSAT_OBC_SENSOR_H

#include <BME280_compensation.h>
#include <SPIBus.h>

#include <array>

#include "osTask.h"
#include "stm32f4xx.h"

namespace hw {

class Sensor : public Noncopyable {
 public:
  struct Buffer {
    std::array<float, 3> array{};
    float& temperature = array[0];
    float& pressure    = array[1];
    float& humidity    = array[2];
  };

  enum ConfigFlags : uint8_t {
    Temperature16Bit      = 0x01,
    FilterOff             = 0x00,
    NormalMode            = 0x03,
    Standby10Ms           = 0x06,
    PressureUltraLowPower = 0x01,
    HumidityStandard      = 0x03,
  };

 public:

  Sensor(const SPIBus& spi, ChipSelect& chipSelect);

  /**
   * Initializes Sensor.
   * Sets size for temperature, pressure and humidity resolution and chosen mode.
   * @param temperatureResolution
   * @param pressureOversampling
   * @param humidityOversampling
   * @param mode
   */
  void init(const ConfigFlags& temperatureResolution, const ConfigFlags& pressureOversampling,
            const ConfigFlags& humidityOversampling, const ConfigFlags& mode);

  /**
   * Sets sensor's configuration.
   * @param standbyTime
   * @param filter
   */
  void configure(const ConfigFlags& standbyTime, const ConfigFlags& filter);

  /**
   *Calls function for reading temperature, pressure and humidity
   */
  void readAll(Buffer& buffer);

 private:
  void getCalibrationData();
  void parseFirstConversionData(const std::array<uint8_t, 1 + 26>& data);
  void parseSecondConversionData(const std::array<uint8_t, 1 + 7>& data);

  /**
   * Writes data in 8-bit format to register
   * @param address register's address
   * @param data
   */
  void write8(uint8_t address, uint8_t data);

 private:
  const SPIBus& spi_;
  ChipSelect& cs_;

  enum Address : uint8_t {

    Calib00       = 0x88,
    Calib26       = 0xE1,
    MainDataBlock = 0xF7,

    HumControl      = 0xF2,
    Config          = 0xF5,
    CTRLmeasAddress = 0xF4
  };

  enum AddressFlag : uint8_t { Write = 0b0111'1111, Read = 0b1111'1111 };

  TempConversionData tempConvData_{};
  PressConversionData pressConvData_{};
  HumidCoversionData humidConvData_{};
};

}  // namespace hw

#endif  // HABSAT_OBC_SENSOR_H
