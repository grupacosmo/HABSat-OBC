//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef HABSAT_OBC_MEASURETIME_H
#define HABSAT_OBC_MEASURETIME_H

#include <Rtc.h>
#include <osTask.h>

namespace services {

class MeasureTime : public Noncopyable {
 public:
  explicit MeasureTime(rtc::Rtc* rtc);
  void init();
  [[nodiscard]] auto getBuffer() const -> const rtc::RtcBuffer&;

 private:
  [[noreturn]] static void measureTimeFunction(void* args);

 private:
  static os::Task measureTask_;
  struct Params {
    rtc::Rtc* rtc;
    rtc::RtcBuffer buffer;
  } params_;
};

}  // namespace services

#endif  // HABSAT_OBC_MEASURETIME_H
