//
// Created by Filip on 15.07.2021.
//

#include <Gps/gpsData.h>

#include <hardware_config.hpp>

#include "Inc/Gps/gpsMessages.hpp"

namespace habsat::gps {

GpsData::GpsData() {
    isDataValid = false;
}

GpsData::GpsData(GpsMessages gpsMessages) {
    processRmcMessage(gpsMessages.Rmc);
    processGgaMessage(gpsMessages.Gga);
}

std::string GpsData::getNameOfPositionFixIndicator(int positionFixIndicator) {
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

double GpsData::convertDegMinToDecDeg(double degMin) {
    double min    = 0.0;
    double decDeg = 0.0;

    // get the minutes, fmod() requires double
    min = fmod((double)degMin, 100.0);

    // rebuild coordinates in decimal degrees
    degMin = (int)(degMin / 100);
    decDeg = degMin + (min / 60);

    return decDeg;
}

void GpsData::processGgaMessage(std::string_view rawMessage) {
    uint8_t idPositionFixIndicator = 0;

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
          &idPositionFixIndicator,
          &this->satellitesUsed,
          &this->hdop,
          &this->mslAltitude,
          &this->mslUnits);

    this->utcHour +=2;
    this->positionFixIndicator = getNameOfPositionFixIndicator(idPositionFixIndicator);
    this->latitude             = convertDegMinToDecDeg(this->latitude);
    this->longitude            = convertDegMinToDecDeg(this->longitude);
}

void GpsData::processRmcMessage(std::string_view rawMessage) {
    char rmcstatus = 'X';

    std::sscanf(
          rawMessage.data(),
          "$GPRMC,%2hhd%2hhd%2hhd.%*d,%c,%*f,%*c,%*f,%*c,%lf,,%2hhd%2hhd%2hhd",
          &this->utcHour,
          &this->utcMin,
          &this->utcSec,
          &rmcstatus,
          &this->speedOverGroundKmPerH,
          &this->day,
          &this->month,
          &this->year);

    this->utcHour += TIME_OFFSET;
    if (rmcstatus == 'A')
        isDataValid = true;
    else
        isDataValid = false;
}
void GpsData::print() {
    std::string latIndicator(1, nsIndicator);
    std::string longIndicator(1, ewIndicator);
    std::string mslUnit(1, mslUnits);
    std::string datavaild = (isDataValid) ? "true" : "false";

    Terminal::pcTransmit("---  Gps Data  ---\r\n");
    Terminal::pcTransmit("Time:\t\t" + std::to_string(utcHour) + ":" + std::to_string(utcMin) + ":" +std::to_string(utcSec) + "\r\n");
    Terminal::pcTransmit("Date:\t\t" + std::to_string(day) + "/" + std::to_string(month) + "/20" +std::to_string(year) + "\r\n");
    Terminal::pcTransmit("Is Data Valid:\t" + datavaild + "\r\n");
    Terminal::pcTransmit("Coordinates:\t" + latIndicator + " " + std::to_string(latitude) + ", " + longIndicator +" " + std::to_string(longitude) + "\r\n");
    Terminal::pcTransmit("Speed:\t\t" + std::to_string(speedOverGroundKmPerH) + " Km/H" + "\r\n");
    Terminal::pcTransmit("Fix Indicator:\t" + positionFixIndicator + "\r\n");
    Terminal::pcTransmit("Number of SAT:\t" + std::to_string(satellitesUsed) + "\r\n");
    Terminal::pcTransmit("HDOP:\t\t" + std::to_string(hdop) + "\r\n");
    Terminal::pcTransmit("MSL Altitude:\t" + std::to_string(mslAltitude) + " " + mslUnit + "\r\n");
}
}  // namespace habsat::gps
