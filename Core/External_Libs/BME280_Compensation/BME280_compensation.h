/*
 * This is a slightly modified version of the code from Bosch BME280 Data Sheet
 */

#ifndef BME280_H
#define BME280_H

#include <cstdio>

namespace bme280 {

struct TempConversionData {
  uint16_t digT1;
  int16_t digT2;
  int16_t digT3;
};

struct PressConversionData {
  uint16_t digP1;
  int16_t digP2;
  int16_t digP3;
  int16_t digP4;
  int16_t digP5;
  int16_t digP6;
  int16_t digP7;
  int16_t digP8;
  int16_t digP9;
};

struct HumidCoversionData {
  uint8_t digH1;
  int16_t digH2;
  uint8_t digH3;
  int16_t digH4;
  int16_t digH5;
  int8_t digH6;
};

// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
// t_fine carries fine temperature as global value
inline int32_t t_fine;
inline int32_t compensateTemp(int32_t adc_T, TempConversionData* calib) {
  int32_t var1, var2, T;
  var1 = ((((adc_T >> 3) - ((int32_t)calib->digT1 << 1))) * ((int32_t)calib->digT2)) >> 11;
  var2 = (((((adc_T >> 4) - ((int32_t)calib->digT1)) * ((adc_T >> 4) - ((int32_t)calib->digT1))) >>
           12) *
          ((int32_t)calib->digT3)) >>
         14;
  t_fine = var1 + var2;
  T      = (t_fine * 5 + 128) >> 8;
  return T;
}

// Returns pressure in Pa as unsigned 32 bit integer in Q24.8 format (24 integer bits and 8
// fractional bits). Output value of “24674867” represents 24674867/256 = 96386.2 Pa = 963.862 hPa
inline uint32_t compensatePressure(int32_t adc_P, PressConversionData* calib) {
  int64_t var1, var2, p;
  var1 = ((int64_t)t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)calib->digP6;
  var2 = var2 + ((var1 * (int64_t)calib->digP5) << 17);
  var2 = var2 + (((int64_t)calib->digP4) << 35);
  var1 = ((var1 * var1 * (int64_t)calib->digP3) >> 8) + ((var1 * (int64_t)calib->digP2) << 12);
  var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib->digP1) >> 33;
  if (var1 == 0) {
    return 0;  // avoid exception caused by division by zero
  }
  p    = 1048576 - adc_P;
  p    = (((p << 31) - var2) * 3125) / var1;
  var1 = (((int64_t)calib->digP9) * (p >> 13) * (p >> 13)) >> 25;
  var2 = (((int64_t)calib->digP8) * p) >> 19;
  p    = ((p + var1 + var2) >> 8) + (((int64_t)calib->digP7) << 4);
  return (uint32_t)p;
}

// Returns humidity in %RH as unsigned 32 bit integer in Q22.10 format (22 integer and 10 fractional
// bits). Output value of “47445” represents 47445/1024 = 46.333 %RH
inline uint32_t compensateHumidity(int32_t adc_H, HumidCoversionData* calib) {
  int32_t v_x1_u32r;
  v_x1_u32r = (t_fine - ((int32_t)76800));
  v_x1_u32r =
      (((((adc_H << 14) - (((int32_t)calib->digH4) << 20) - (((int32_t)calib->digH5) * v_x1_u32r)) +
         ((int32_t)16384)) >>
        15) *
       (((((((v_x1_u32r * ((int32_t)calib->digH6)) >> 10) *
            (((v_x1_u32r * ((int32_t)calib->digH3)) >> 11) + ((int32_t)32768))) >>
           10) +
          ((int32_t)2097152)) *
             ((int32_t)calib->digH2) +
         8192) >>
        14));
  v_x1_u32r = (v_x1_u32r -
               (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)calib->digH1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
  return (uint32_t)(v_x1_u32r >> 12);
}

}  // namespace bme280

#endif