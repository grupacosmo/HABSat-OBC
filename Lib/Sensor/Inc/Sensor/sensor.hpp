//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#ifndef HABSAT_OBC_SENSOR_H
#define HABSAT_OBC_SENSOR_H

#include <BME280_compensation.h>

#include <array>

#include "Buses/spi.hpp"

namespace habsat::sensor {

namespace details {

enum Address : uint8_t {
    Calib00       = 0x88,
    Calib26       = 0xE1,
    MainDataBlock = 0xF7,

    HumControl      = 0xF2,
    Config          = 0xF5,
    CTRLMeasAddress = 0xF4
};

enum AddressFlag : uint8_t {
    Write = 0b0111'1111,
    Read  = 0b1111'1111,
};

}  // namespace details

// TODO add missing options to enums
enum TemperatureResolution : uint8_t { SixteenBit = 0x01 };

enum PressureOversampling : uint8_t { UltraLowPower = 0x01 };

enum HumidityOversampling : uint8_t { Standard = 0x03 };

enum Mode : uint8_t { Normal = 0x03 };

enum StandbyTime : uint8_t { TenMs = 0x06 };

enum Filter : uint8_t { Off = 0x00 };

struct Settings {
    TemperatureResolution temperatureResolution;
    PressureOversampling pressureOversampling;
    HumidityOversampling humidityOversampling;
    Mode mode;
    StandbyTime standbyTime;
    Filter filter;
};

struct Buffer {
    std::array<float, 3> array{};
    float& temperature = array[0];
    float& pressure    = array[1];
    float& humidity    = array[2];
};

class Sensor : public utils::Noncopyable {
   public:
    Sensor(const buses::SPI& spi, mcuBoard::GPIOPin& chipSelect);

    /**
     * Initializes Sensor.
     * Sets size for temperature, pressure and humidity resolution and chosen mode.
     * @param temperatureResolution
     * @param pressureOversampling
     * @param humidityOversampling
     * @param mode
     * @param standbyTime
     * @param filter
     */
    void init(const Settings& settings);

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
    const buses::SPI& spi_;
    mcuBoard::GPIOPin& cs_;

    bme280::TempConversionData tempConvData_{};
    bme280::PressConversionData pressConvData_{};
    bme280::HumidCoversionData humidConvData_{};
};

}  // namespace habsat::sensor

#endif  // HABSAT_OBC_SENSOR_H
