//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "sensor.h"
#include "../OBC/obc.h"
#include "../Utils/Bitwise_Operations/bitwise_operations.h"

namespace hw
{

Sensor::Sensor(const SPIBus* spi) : spi_(spi) {}

void Sensor::init(const ConfigFlags & temperature_resolution, const ConfigFlags & pressure_oversampling, const ConfigFlags & humidity_oversampling, const ConfigFlags & mode)
{
    spi_->clearCS(cs_);
    HAL_Delay(5);

    for(size_t i = 0; i < t_.size(); ++i)
        t_[i] = bitwise::swapBytes(read16(digT_[i]));

    for(size_t i = 0; i < p_.size(); ++i)
        p_[i] = bitwise::swapBytes(read16(digP_[i]));

    h_[0] = read8(digH_[0]);
    h_[1] = ((uint16_t)read8(digH_[1] + 1) << 8) | (uint16_t)read8(digH_[1]);//bitwise::swapBytes(read16(DIG_H[1]));
    h_[2] = read8(digH_[2]);
    h_[3] = ((uint16_t)read8(digH_[3]) * 16 ) | (read8(digH_[3]+1) & 0x0F);
    h_[4] = ((uint16_t)read8(digH_[4] + 1) * 16 ) | (read8(digH_[4]) >> 4);
    h_[5] = read8(digH_[5]);


    const uint8_t HumReg = (read8(Address::HumControl) & 0xF8 ) | humidity_oversampling;

    write8(Address::HumControl, HumReg);
    write8(0xF4, (temperature_resolution << 5 ) | (pressure_oversampling << 2) | mode);
    spi_->setCS(cs_);
}

void Sensor::configure(const ConfigFlags& standby_time, const ConfigFlags& filter)
{
    const uint8_t data = ((standby_time & 0x07) << 5) | (((filter & 0x07) << 2) & 0xFC);
    spi_->clearCS(cs_);
    write8(Address::Config, data);
    spi_->setCS(cs_);
}

void Sensor::readAll()
{
    spi_->clearCS(cs_);
    buffers_.temperature = readTemperature();
    buffers_.pressure = readPressure();
    buffers_.humidity = readHumidity();
    spi_->setCS(cs_);
}

float Sensor::readTemperature()
{
    auto adc_T = read24(Address::TempData);
    return convert_data_temperature(adc_T);
}

float Sensor::readPressure()
{
    auto adc_P = read24(Address::PressureData);
    return convert_data_pressure(adc_P) / 100;
}

float Sensor::readHumidity()
{
    auto adc_H = read16(Address::HumidityData);
    return convert_data_humidity(adc_H);
}

void Sensor::write8(const uint8_t address, const uint8_t data)
{
    std::array<uint8_t, 2> bytes = {bitwise::clearBits<7>(address), data};
    spi_->transmit(bytes.data(), bytes.size());
}

uint8_t Sensor::read8(const uint8_t address)
{
    std::array<uint8_t, 2> bytes = {bitwise::setBits<7>(address)};
    spi_->transmitAndReceive(bytes.data(), bytes.data(), bytes.size());
    return bytes[1];
}

uint16_t Sensor::read16(const uint8_t address)
{
    std::array<uint8_t, 3> bytes = {bitwise::setBits<7>(address)};
    spi_->transmitAndReceive(bytes.data(), bytes.data(), bytes.size());
    return (bytes[1] << 8) | (bytes[2]);
}

uint32_t Sensor::read24(const uint8_t address)
{
    std::array<uint8_t, 4> bytes = {bitwise::setBits<7>(address)};
    spi_->transmitAndReceive(bytes.data(), bytes.data(), bytes.size());
    return (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]);
}

void Sensor::write8(const Address& address, const uint8_t data)
{
    write8(static_cast<uint8_t>(address), data);
}

uint8_t Sensor::read8(const Address& address)
{
    return read8(static_cast<uint8_t>(address));
}

uint16_t Sensor::read16(const Address& address)
{
    return read16(static_cast<uint8_t>(address));
}

uint32_t Sensor::read24(const Address& address)
{
    return read24(static_cast<uint8_t>(address));
}




void Sensor::measureTaskFunction(void *args)
{
    (void)args;
    Sensor& sensor = obc().hardware.sensor;

    while(true)
    {
        sensor.readAll();
        os::Task::delay(256);
    }
}

const os::Task &Sensor::getMeasureTask() const
{
    return measureTask_;
}

const Sensor::Buffers &Sensor::getBuffers() const
{
    return buffers_;
}

float Sensor::convert_data_temperature(int32_t adc_T)
{
    // this is taken from the BME280 datasheet
    adc_T >>= 4;

    const int32_t var1 = ((((adc_T>>3) - ((int32_t) t_[0] << 1))) * ((int32_t) t_[1])) >> 11;
    const int32_t var2 = (((((adc_T>>4) - ((int32_t) t_[0])) * ((adc_T >> 4) - ((int32_t) t_[0]))) >> 12) * ((int32_t) t_[2])) >> 14;
    tFine_ = var1 + var2;
    float T = (tFine_ * 5 + 128) >> 8;
    return T / 100;
}

uint32_t Sensor::convert_data_pressure(int32_t adc_P){

    adc_P >>= 4;
    // this is taken from the BME280 datasheet
    int64_t var1 = ((int64_t) tFine_) - 128000;
    int64_t var2 = var1 * var1 * (int64_t) p_[5];
    var2 = var2 + ((var1*(int64_t) p_[4]) << 17);
    var2 = var2 + (((int64_t) p_[3]) << 35);
    var1 = ((var1 * var1 * (int64_t) p_[2]) >> 8) + ((var1 * (int64_t) p_[1]) << 12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t) p_[0]) >> 33;
    if (var1 == 0) return 0;
    int64_t P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;
    var1 = (((int64_t) p_[8]) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((int64_t) p_[7]) * P) >> 19;
    P = ((P + var1 + var2) >> 8) + (((int64_t) p_[6]) << 4);
    return (uint32_t)P / 256;

}

uint32_t Sensor::convert_data_humidity(int32_t adc_H)
{
    constexpr int32_t humidity_max = 102400;

    int32_t var1 = tFine_ - ((int32_t)76800);
    int32_t var2 = (int32_t)(adc_H * 16384);
    int32_t var3 = (int32_t)(((int32_t) h_[3]) * 1048576);
    int32_t var4 = ((int32_t) h_[4]) * var1;
    int32_t var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t) h_[5])) / 1024;
    var3 = (var1 * ((int32_t) h_[2])) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t) h_[1])) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t) h_[0])) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    int32_t humidity = (uint32_t)(var5 / 4096);

    if (humidity > humidity_max)
    {
        humidity = humidity_max;
    }

    return humidity / 1024;
}

}