//
// Created by Filip on 15.07.2021.
//

#ifndef HABSAT_OBC_GPSDATA_H
#define HABSAT_OBC_GPSDATA_H

#include <math.h>

#include <cstdint>
#include <string>
#include "gpsMessages.hpp"
namespace habsat::gps {
// NVME type RMC message structure




struct GpsData {
    // Documentation
    // https://www.sparkfun.com/datasheets/GPS/NMEA%20Reference%20Manual1.pdf

   private:
    int TIME_OFFSET = 2;    //UTC+2
    char rmcStatus = 'X';
   public:
    // UTC time
    uint8_t utcHour{};
    uint8_t utcMin{};
    uint8_t utcSec{};

    bool isDataValid{};    // status of message (data are valid =A or not = V)
    double latitude{};    // ddmm.mmmm
    char nsIndicator{};  // north of South indicator  (N or S)
    double longitude{};   // In
    char ewIndicator{};  // west of East indicator  (E or W)
    double speedOverGroundKmPerH{};

    // date
    uint8_t day{};
    uint8_t month{};
    uint8_t year{};

    uint8_t positionFixIndicator;
    uint8_t satellitesUsed{};
    double hdop{};
    double mslAltitude{};
    char mslUnits{};

    GpsData();
    explicit GpsData(const GpsMessages& gpsMessages);
    static auto getLabelPositionFiXIndicator(uint8_t positionFixIndicator) -> std::string;


   private:

    void processGgaMessage(std::string_view rawMessage);
    void processRmcMessage(std::string_view rawMessage);
    static auto convertDegMinToDecDeg (double degMin) -> double;
};
}  // namespace habsat::gps

#endif  // HABSAT_OBC_GPSDATA_H
