//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_MEASURETIME_H
#define RCC_SYS_MEASURETIME_H

#include <Rtc.h>
#include <osTask.h>

namespace services {

class MeasureTime : public Noncopyable {
 public:
  explicit MeasureTime(hw::Rtc* rtc);
  void init();
  [[nodiscard]] auto getBuffer() const -> const hw::Rtc::Buffer&;

 private:
  static void measureTimeFunction(void* args);

 private:
  static os::Task measureTask_;
  struct Params {
    hw::Rtc* rtc_;
    hw::Rtc::Buffer buffer_;
    explicit Params(hw::Rtc* rtc) : rtc_(rtc) {}
  } params_;
};

}  // namespace services

#endif  // RCC_SYS_MEASURETIME_H
