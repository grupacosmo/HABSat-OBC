//
// Created by Filip on 15.07.2021.
//



#include "Inc/Gps/gpsData.h"

#include <string>

namespace habsat::gps {

GpsData::GpsData() : utcHour{0}, utcMin{0}, utcSec{0},isDataValid{false}, day{0},month{0},year{0}{}

GpsData::GpsData(const GpsMessages& gpsMessages) {
    this->rmcStatus = 'x';
    processRmcMessage(gpsMessages.Rmc);
    processGgaMessage(gpsMessages.Gga);

    if(rmcStatus == 'A' || positionFixIndicator == 1 || positionFixIndicator == 2 || positionFixIndicator == 6)
        isDataValid = true;
    else
        isDataValid = false;

    this->utcHour += TIME_OFFSET;

}

auto GpsData::getLabelPositionFiXIndicator(uint8_t positionFixIndicator) -> std::string {
    switch (positionFixIndicator) {
        case 0:
            return "Fix not available or invalid";
        case 1:
            return "GPS SPS Mode, fix valid";
        case 2:
            return "Differential GPS, SPS Mode, fix valid";
        case 3:
        case 5:
            return "Not supported";
        case 6:
            return "Dead Reckoning Mode, fix valid";
        default:
            return "Code not recognized";
    }
}

auto GpsData::convertDegMinToDecDeg(double degMin) -> double {
    double min    = 0.0;
    double decDeg = 0.0;

    // get the minutes, fmod() requires double
    min = fmod(static_cast<double>(degMin), 100.0);

    // rebuild coordinates in decimal degrees
    degMin = static_cast<int>((degMin / 100));
    decDeg = degMin + (min / 60);

    return decDeg;
}

void GpsData::processGgaMessage(std::string_view rawMessage) {
    sscanf(
          rawMessage.data(),
          "$GPGGA,%2hhd%2hhd%2hhd.%*d,%lf,%c,%lf,%c,%hhd,%hhd,%lf,%lf,%c\r\n",
          &this->utcHour,
          &this->utcMin,
          &this->utcSec,
          &this->latitude,
          &this->nsIndicator,
          &this->longitude,
          &this->ewIndicator,
          &this->positionFixIndicator,
          &this->satellitesUsed,
          &this->hdop,
          &this->mslAltitude,
          &this->mslUnits);

    this->latitude             = convertDegMinToDecDeg(this->latitude);
    this->longitude            = convertDegMinToDecDeg(this->longitude);
}

void GpsData::processRmcMessage(std::string_view rawMessage) {
    std::sscanf(
          rawMessage.data(),
          "$GPRMC,%2hhd%2hhd%2hhd.%*d,%c,%*f,%*c,%*f,%*c,%lf,,%2hhd%2hhd%2hhd",
          &this->utcHour,
          &this->utcMin,
          &this->utcSec,
          &this->rmcStatus,
          &this->speedOverGroundKmPerH,
          &this->day,
          &this->month,
          &this->year);
}
}  // namespace habsat::gps
