//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "MeasureTime.h"

namespace services {

os::Task MeasureTime::measureTask_{"rtc_task", 128, os::Priority::Idle, measureTimeFunction};

MeasureTime::MeasureTime(rtc::Rtc *rtc)
    : params_{.rtc = rtc, .buffer{}}
{

}

void MeasureTime::init()
{
    measureTask_.addToScheduler(static_cast<void*>(&params_));
}

void MeasureTime::measureTimeFunction(void *args)
{
    auto params = static_cast<MeasureTime::Params*>(args);
    while(true)
    {
        params->rtc->readTimeAndDate(params->buffer);
        os::Task::delay(500);
    }
}
auto MeasureTime::getBuffer() const -> const rtc::RtcBuffer &
{
    return params_.buffer;
}

}