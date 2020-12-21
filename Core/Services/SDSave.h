//
// Created by Jakub Zygmunt on 06.12.2020.
//

#ifndef HABSAT_OBC_SDSAVE_H
#define HABSAT_OBC_SDSAVE_H

#include <array>

#include "SDReader.h"
#include "Rtc.h"
#include "Sensor.h"
#include "osTask.h"

namespace services {

class SDSave : public Noncopyable {
 public:
  SDSave(hw::SDReader* sdReader, const hw::Sensor::Buffer* sensorBuffer, const hw::Rtc::Buffer* rtcBuffer);
  void init();

 private:
  template<size_t PathLength>
  static void prepareFileHeader(std::array<char, PathLength>&buffer);

  template<size_t PathLength>
  static void prepareRTCData(std::array<char, PathLength>&buffer, const hw::Rtc::Buffer* buf);

  template<size_t PathLength>
  static void prepareSensorData(std::array<char, PathLength>&buffer, const hw::Sensor::Buffer* buf);
  [[noreturn]] static void sdSaveTaskFunction(void* args);

 private:
  static os::Task sdSaveTask_;
  struct Params {
    hw::SDReader* sdReader_;
    const hw::Sensor::Buffer* sensorBuffer_;
    const hw::Rtc::Buffer* rtcBuffer_;
    Params(hw::SDReader* sdReader, const hw::Sensor::Buffer* sensorBuffer, const hw::Rtc::Buffer* rtcBuffer)
        : sdReader_(sdReader), sensorBuffer_(sensorBuffer), rtcBuffer_(rtcBuffer) {}
  } params_;
};

#include "SDSave.ipp"

}  // namespace services
#endif  // HABSAT_OBC_SDSAVE_H
