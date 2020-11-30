//
// Created by Wiktor Wieclaw on 11/30/20.
//

#ifndef RCC_SYS_MEASURETIME_H
#define RCC_SYS_MEASURETIME_H

#include <os_task.h>
#include <rtc.h>

namespace services {

class MeasureTime
{
public:
    explicit MeasureTime(hw::Rtc* rtc);
    void init();
    const hw::Rtc::Buffer & getBuffer() const;
private:
    static void measureTimeFunction(void *args);
private:
    static os::Task measureTask_;
    struct Params
    {
        hw::Rtc *rtc_;
        hw::Rtc::Buffer buffer_;
        Params(hw::Rtc* rtc) : rtc_(rtc) { }
    } params_;
};

}

#endif//RCC_SYS_MEASURETIME_H
