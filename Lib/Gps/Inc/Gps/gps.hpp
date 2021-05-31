//
// Created by Filip on 20.04.2021.
//

#ifndef HABSAT_OBC_GPS_H
#define HABSAT_OBC_GPS_H

#include <vector>

#include "Buses/uart.hpp"

namespace habsat::gps {

// NVME type RMC message structure
struct GpsBuffer{
    // Documentation
    // https://www.sparkfun.com/datasheets/GPS/NMEA%20Reference%20Manual1.pdf

    // UTC time
    uint8_t utcHour;
    uint8_t utcMin;
    uint8_t utcSec;
    uint16_t utcMicroSec;

    char rmcStatus;                    // dtatus of message (data are valid =A or not = V)
    float latitude;                 // ddmm.mmmm
    char nsIndicator;               // north of South indicator  (N or S)
    float longitude;                // In
    char ewIndicator;               // west of East indicator  (E or W)
    double speedOverGroundKmPerH;
    double courseOverGroundDegrees;

    // date
    uint8_t day;
    uint8_t month;
    uint8_t year;

    char mode;          // Mode  A= Autonomus D=DGPS, E=DR
    char* checksum[5];  // Checksum of all chars between $ (start body message) and * (end body message)
};

//struct just for debugging purposes
//TODO check message size and change to std::array
struct GpsData{
    std::string Rmc;
    std::string Vtg;
    std::string Gga;
    std::string Gsa;
    std::string Gsv;
    std::string Gll;
};

enum class MessageType { Rmc, Vtg, Gga, Gsa, Gsv, Gll, None };

class Gps {
   public:
    Gps(UART_HandleTypeDef& uart, DMA_HandleTypeDef& dma_usart_rx);
    void init();
    void callback();
    auto getData() -> GpsData;

   private:
    static const int rawBufferSize = 250;
    UART_HandleTypeDef& uart;
    DMA_HandleTypeDef& dmaRx;
    //TODO check max message size and change to std::array
    std::string rawMessageBuffor;
    uint8_t rawBuffor[rawBufferSize];

    GpsData GpsMessages;
    GpsBuffer gpsBuffer;

    void processGgaMessage(std::string_view rawMessage);
    void processRmcMessage(std::string_view rawMessage);
    auto retrieveSingleMessage(std::string& rawMessage) -> GpsData;
};

}  // namespace habsat::gps

#endif  // HABSAT_OBC_GPS_H
