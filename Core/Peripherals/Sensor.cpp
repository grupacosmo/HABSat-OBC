//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "Sensor.h"

#include "bitwiseOperations.h"

namespace sensor {

using namespace impl;

Sensor::Sensor(const mcuBoard::SPIBus& spi, mcuBoard::ChipSelect& cs) : spi_(spi), cs_(cs) {}

void Sensor::init(
      const ConfigFlags& temperatureResolution,
      const ConfigFlags& pressureOversampling,
      const ConfigFlags& humidityOversampling,
      const ConfigFlags& mode) {
    getCalibrationData();
    write8(Address::HumControl & AddressFlag::Write, humidityOversampling);
    write8(
          Address::CTRLMeasAddress & AddressFlag::Write,
          (temperatureResolution << 5) | (pressureOversampling << 2) | mode);
}

void Sensor::getCalibrationData() {
    // BME280 documentation page 24 table 16
    std::array<uint8_t, 1 + 26> calibData1{Address::Calib00 & AddressFlag::Read};
    spi_.transmitAndReceive(cs_, calibData1);
    parseFirstConversionData(calibData1);

    std::array<uint8_t, 1 + 7> calibData2{Address::Calib26 & AddressFlag::Read};
    spi_.transmitAndReceive(cs_, calibData2);
    parseSecondConversionData(calibData2);
}

void Sensor::parseFirstConversionData(const std::array<uint8_t, 1 + 26>& data) {
    // BME280 documentation page 24 table 16
    using bitwise::concat2Bytes;

    tempConvData_.digT1 = concat2Bytes(data[2], data[1]);
    tempConvData_.digT2 = concat2Bytes(data[4], data[3]);
    tempConvData_.digT3 = concat2Bytes(data[6], data[5]);

    pressConvData_.digP1 = concat2Bytes(data[8], data[7]);
    pressConvData_.digP2 = concat2Bytes(data[10], data[9]);
    pressConvData_.digP3 = concat2Bytes(data[12], data[11]);
    pressConvData_.digP4 = concat2Bytes(data[14], data[13]);
    pressConvData_.digP5 = concat2Bytes(data[16], data[15]);
    pressConvData_.digP6 = concat2Bytes(data[18], data[17]);
    pressConvData_.digP7 = concat2Bytes(data[20], data[19]);
    pressConvData_.digP8 = concat2Bytes(data[22], data[21]);
    pressConvData_.digP9 = concat2Bytes(data[24], data[23]);

    humidConvData_.digH1 = data[26];
}

void Sensor::parseSecondConversionData(const std::array<uint8_t, 1 + 7>& data) {
    // BME280 documentation page 24 table 16
    humidConvData_.digH2 = bitwise::concat2Bytes(data[2], data[1]);
    humidConvData_.digH3 = data[3];
    humidConvData_.digH4 = (data[4] << 4) | bitwise::nibbleLow(data[5]);
    humidConvData_.digH5 = (data[6] << 4) | (data[5] >> 4);
    humidConvData_.digH6 = data[7];
}

void Sensor::configure(const ConfigFlags& standbyTime, const ConfigFlags& filter) {
    const uint8_t data = ((standbyTime & 0x07) << 5) | (((filter & 0x07) << 2) & 0xFC);
    write8(Address::Config, data);
}

void Sensor::readAll(Buffer& buffer) {
    // BME280 documentation page 31 table 29, 30, 31
    using bitwise::concat2Bytes;
    using bitwise::concat3Bytes;

    uint8_t address = Address::MainDataBlock & AddressFlag::Read;
    std::array<uint8_t, 9> bytes{address};
    spi_.transmitAndReceive(cs_, bytes);

    const auto rawPress = concat3Bytes(bytes[1], bytes[2], bytes[3]) >> 4;
    const auto rawTemp  = concat3Bytes(bytes[4], bytes[5], bytes[6]) >> 4;
    const auto rawHumid = concat2Bytes(bytes[7], bytes[8]);

    // BME280 documentation page 25 point 4.2.3
    buffer.temperature = bme280::compensateTemp(rawTemp, &tempConvData_) / 100.0f;
    buffer.pressure    = bme280::compensatePressure(rawPress, &pressConvData_) / 25600.0f;
    buffer.humidity    = bme280::compensateHumidity(rawHumid, &humidConvData_) / 1024.0f;
}

void Sensor::write8(const uint8_t address, const uint8_t data) {
    std::array<uint8_t, 2> bytes = {address, data};
    spi_.transmit(cs_, bytes);
}

}  // namespace sensor