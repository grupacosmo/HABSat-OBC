//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "Sensor.h"
#include "bitwiseOperations.h"
#include "Obc.h"

namespace hw
{

Sensor::Sensor(const SPIBus& spi, ChipSelect& cs) : spi_(spi), cs_(cs) {}

void Sensor::init(const ConfigFlags & temperature_resolution, const ConfigFlags & pressure_oversampling, const ConfigFlags & humidity_oversampling, const ConfigFlags & mode)
{
    getCalibrationData();
    write8(Address::HumControl, humidity_oversampling);
    write8(Address::CTRLmeasAddress, (temperature_resolution << 5 ) | (pressure_oversampling << 2) | mode);
}

void Sensor::getCalibrationData()
{
    // BME280 documentation page 24 table 16
    std::array<uint8_t, 1 + 26> calibData1{Address::Calib00 & AddressFlag::Read};
    spi_.transmitAndReceive(cs_, calibData1.data(), calibData1.data(), calibData1.size());
    parseFirstConversionData(calibData1);

    std::array<uint8_t, 1 + 7> calibData2{Address::Calib26 & AddressFlag::Read};
    spi_.transmitAndReceive(cs_, calibData2.data(), calibData2.data(), calibData2.size());
    parseSecondConversionData(calibData2);
}

void Sensor::parseFirstConversionData(const std::array<uint8_t, 1 + 26> &dataBlock)
{
    // BME280 documentation page 24 table 16
    tempConvData_.digT1  = bitwise::concat2Bytes(dataBlock[2], dataBlock[1]);
    tempConvData_.digT2  = bitwise::concat2Bytes(dataBlock[4], dataBlock[3]);
    tempConvData_.digT3  = bitwise::concat2Bytes(dataBlock[6], dataBlock[5]);

    pressConvData_.digP1 = bitwise::concat2Bytes(dataBlock[8], dataBlock[7]);
    pressConvData_.digP2 = bitwise::concat2Bytes(dataBlock[10], dataBlock[9]);
    pressConvData_.digP3 = bitwise::concat2Bytes(dataBlock[12], dataBlock[11]);
    pressConvData_.digP4 = bitwise::concat2Bytes(dataBlock[14], dataBlock[13]);
    pressConvData_.digP5 = bitwise::concat2Bytes(dataBlock[16], dataBlock[15]);
    pressConvData_.digP6 = bitwise::concat2Bytes(dataBlock[18], dataBlock[17]);
    pressConvData_.digP7 = bitwise::concat2Bytes(dataBlock[20], dataBlock[19]);
    pressConvData_.digP8 = bitwise::concat2Bytes(dataBlock[22], dataBlock[21]);
    pressConvData_.digP9 = bitwise::concat2Bytes(dataBlock[24], dataBlock[23]);

    humidConvData_.digH1 = dataBlock[26];
}

void Sensor::parseSecondConversionData(const std::array<uint8_t, 1 + 7>& dataBlock)
{
    // BME280 documentation page 24 table 16
    humidConvData_.digH2 = bitwise::concat2Bytes(dataBlock[2], dataBlock[1]);
    humidConvData_.digH3 = dataBlock[3];
    humidConvData_.digH4 = (dataBlock[4] << 4) | bitwise::nibbleLow(dataBlock[5]);
    humidConvData_.digH5 = (dataBlock[6] << 4) | (dataBlock[5] >> 4);
    humidConvData_.digH6 = dataBlock[7];
}

void Sensor::configure(const ConfigFlags& standby_time, const ConfigFlags& filter)
{
    const uint8_t data = ((standby_time & 0x07) << 5) | (((filter & 0x07) << 2) & 0xFC);
    write8(Address::Config, data);
}

void Sensor::readAll(Buffer& buffer)
{
    // BME280 documentation page 31 table 29, 30, 31
    uint8_t address = Address::MainDataBlock & AddressFlag::Read;
    std::array<uint8_t, 9> bytes{address};
    spi_.transmitAndReceive(cs_, bytes.data(), bytes.data(), bytes.size());

    const auto rawPress = bitwise::concat3Bytes(bytes[1], bytes[2], bytes[3]) >> 4;
    const auto rawTemp = bitwise::concat3Bytes(bytes[4], bytes[5], bytes[6]) >> 4;
    const auto rawHumid = bitwise::concat2Bytes(bytes[7], bytes[8]);

    // BME280 documentation page 25 point 4.2.3
    buffer.temperature = BME280_compensate_T(rawTemp, &tempConvData_) / 100.0;
    buffer.pressure = BME280_compensate_P(rawPress, &pressConvData_) / 25600.0;
    buffer.humidity = BME280_compensate_H(rawHumid, &humidConvData_) / 1024.0;
}

void Sensor::write8(const uint8_t address, const uint8_t data)
{
    std::array<uint8_t, 2> bytes = {address, data};
    spi_.transmit(cs_, bytes.data(), bytes.size());
}

}