//
// Created by Agnieszka Rejczak on 8/23/2020.
//

#include "sensor.h"
#include "../OBC/obc.h"
#include "../Utils/Bitwise_Operations/bitwise_operations.h"

namespace hw
{

Sensor::Sensor(const SPIBus* spi) : spi(spi) {}

void Sensor::init(const ConfigFlags & temperature_resolution, const ConfigFlags & pressure_oversampling, const ConfigFlags & humidity_oversampling, const ConfigFlags & mode)
{
    spi->clearCS(cs);
    HAL_Delay(5);

    for(size_t i = 0; i < t.size(); ++i)
        t[i] = bitwise::swapBytes(read16(DIG_T[i]));

    for(size_t i = 0; i < p.size(); ++i)
        p[i] = bitwise::swapBytes(read16(DIG_P[i]));

    h[0] = read8(DIG_H[0]);
    h[1] = ((uint16_t)read8(DIG_H[1] + 1) << 8) | (uint16_t)read8(DIG_H[1]);//bitwise::swapBytes(read16(DIG_H[1]));
    h[2] = read8(DIG_H[2]);
    h[3] = ((uint16_t)read8(DIG_H[3]) * 16 ) | (read8(DIG_H[3]+1) & 0x0F);
    h[4] = ((uint16_t)read8(DIG_H[4] + 1) * 16 ) | (read8(DIG_H[4]) >> 4);
    h[5] = read8(DIG_H[5]);


    const uint8_t HumReg = (read8(Address::BME280_HUM_CONTROL) & 0xF8 ) | humidity_oversampling;

    write8(Address::BME280_HUM_CONTROL, HumReg);
    write8(0xF4, (temperature_resolution << 5 ) | (pressure_oversampling << 2) | mode);
    spi->setCS(cs);
}

void Sensor::configure(const ConfigFlags& standby_time, const ConfigFlags& filter)
{
    const uint8_t data = ((standby_time & 0x07) << 5) | (((filter & 0x07) << 2) & 0xFC);
    write8(Address::BME280_CONFIG, data);
}

void Sensor::readAll()
{
    spi->clearCS(cs);
    buffers.temperature = readTemperature();
    buffers.pressure = readPressure();
    buffers.humidity = readHumidity();
    spi->setCS(cs);
}

float Sensor::readTemperature()
{
    auto adc_T = read24(Address::BME280_TEMP_DATA);
    return convert_data_temperature(adc_T);
}

float Sensor::readPressure()
{
    auto adc_P = read24(Address::BME280_PRESSURE_DATA);
    return convert_data_pressure(adc_P) / 100;
}

float Sensor::readHumidity()
{
    auto adc_H = read16(Address::BME280_HUMID_DATA);
    return convert_data_humidity(adc_H);
}

void Sensor::write8(const uint8_t address, const uint8_t data)
{
    std::array<uint8_t, 2> bytes = {bitwise::clearBits(address, 7), data};
    spi->transmit(bytes.data(), bytes.size());
}

uint8_t Sensor::read8(const uint8_t address)
{
    std::array<uint8_t, 2> bytes = {bitwise::setBits(address, 7)};
    spi->transmitAndReceive(bytes.data(), bytes.data(), bytes.size());
    return bytes[1];
}

uint16_t Sensor::read16(const uint8_t address)
{
    std::array<uint8_t, 3> bytes = {bitwise::setBits(address, 7)};
    spi->transmitAndReceive(bytes.data(), bytes.data(), bytes.size());
    return (bytes[1] << 8) | (bytes[2]);
}

uint32_t Sensor::read24(const uint8_t address)
{
    std::array<uint8_t, 4> bytes = {bitwise::setBits(address, 7)};
    spi->transmitAndReceive(bytes.data(), bytes.data(), bytes.size());
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
    Sensor& sensor = obc.hardware.sensor;

    while(true)
    {
        sensor.readAll();
        os::Task::delay(256);
    }
}

const os::Task &Sensor::getMeasureTask() const
{
    return measure_task;
}

const Sensor::Buffers &Sensor::getBuffers() const
{
    return buffers;
}

float Sensor::convert_data_temperature(int32_t adc_T)
{
    // this is taken from the BME280 datasheet
    adc_T >>= 4;

    const int32_t var1 = ((((adc_T>>3) - ((int32_t)t[0] << 1))) * ((int32_t)t[1])) >> 11;
    const int32_t var2 = (((((adc_T>>4) - ((int32_t)t[0])) * ((adc_T >> 4) - ((int32_t)t[0]))) >> 12) * ((int32_t)t[2])) >> 14;
    t_fine = var1 + var2;
    float T = (t_fine * 5 + 128) >> 8;
    return T / 100;
}

uint32_t Sensor::convert_data_pressure(int32_t adc_P){

    adc_P >>= 4;
    // this is taken from the BME280 datasheet
    int64_t var1 = ((int64_t)t_fine) - 128000;
    int64_t var2 = var1 * var1 * (int64_t)p[5];
    var2 = var2 + ((var1*(int64_t)p[4]) << 17);
    var2 = var2 + (((int64_t)p[3]) << 35);
    var1 = ((var1 * var1 * (int64_t)p[2]) >> 8) + ((var1 * (int64_t)p[1]) << 12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)p[0]) >> 33;
    if (var1 == 0) return 0;
    int64_t P = 1048576 - adc_P;
    P = (((P << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)p[8]) * (P >> 13) * (P >> 13)) >> 25;
    var2 = (((int64_t)p[7]) * P) >> 19;
    P = ((P + var1 + var2) >> 8) + (((int64_t)p[6]) << 4);
    return (uint32_t)P / 256;

}

uint32_t Sensor::convert_data_humidity(int32_t adc_H)
{
    constexpr int32_t humidity_max = 102400;

    int32_t var1 = t_fine - ((int32_t)76800);
    int32_t var2 = (int32_t)(adc_H * 16384);
    int32_t var3 = (int32_t)(((int32_t)h[3]) * 1048576);
    int32_t var4 = ((int32_t)h[4]) * var1;
    int32_t var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    var2 = (var1 * ((int32_t)h[5])) / 1024;
    var3 = (var1 * ((int32_t)h[2])) / 2048;
    var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    var2 = ((var4 * ((int32_t)h[1])) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((int32_t)h[0])) / 16);
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