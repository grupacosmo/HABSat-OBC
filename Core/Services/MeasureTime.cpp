//
// Created by Wiktor Wieclaw on 11/30/20.
//

#include "MeasureTime.h"

namespace services {

os::Task MeasureTime::measureTask_{"rtc_task", 128, os::Priority::Idle, measureTimeFunction};

MeasureTime::MeasureTime(hw::Rtc *rtc)
    : params_(rtc)
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
        params->rtc_->readTimeAndDate(params->buffer_);
        os::Task::delay(500);
    }
}
const hw::Rtc::Buffer &MeasureTime::getBuffer() const
{
    return params_.buffer_;
}

}