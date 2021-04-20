//
// Created by Filip on 20.04.2021.
//

#ifndef HABSAT_OBC_GPS_H
#define HABSAT_OBC_GPS_H

#include "Buses/uart.hpp"

namespace habsat::gps{
//NVME type GGA message structure
typedef struct
{
    //Documentation - GGA —Global Positioning System Fixed Data, page 2
    //https://www.sparkfun.com/datasheets/GPS/NMEA%20Reference%20Manual1.pdf

    /*ignored */
    //char* MessageID[8]; (message protocol type)

    //UTC_Time
    uint8_t  UTC_Hour;
    uint8_t  UTC_Min;
    uint8_t  UTC_Sec;
    uint16_t UTC_MicroSec;

    char	  Status;                 //Status of message (data are valid =A or not = V)
    double  Latitude;               //ddmm.mmmm
    char    NS_Indicator;           //North of South indicator  (N or S)
    double  Longitude;              //In
    char    EW_Indicator;           //West of East indicator  (E or W)
    double  SpeedOverGroundKmPerH;
    double  CourseOverGroundDegrees;

    //Date
    uint8_t Day;
    uint8_t Month;
    uint8_t Year;

    /*ignored because no information in documentation*/
    //double  MagneticVariation;       //degrees E=east or W=west, East/West Indicator redundand

    char    Mode;                    //Mode  A= Autonomus D=DGPS, E=DR
    char* checksum[5];               //Checksum of all chars between $ (start body message) and * (end body message)

}GPGGA_t;

class Gps {
   public:
    Gps(UART_HandleTypeDef* uart, DMA_HandleTypeDef* dma_usart_rx);
    void init();
    void callback();


   private:

    static const int rawMessageBufferSize = 80;
    static const int rawBufferSize = 100;
    UART_HandleTypeDef* uart;
    DMA_HandleTypeDef* dma_rx;
    std::array<uint8_t,rawBufferSize> rawMessageBuffor;
    uint8_t rawBuffor[rawBufferSize];
    GPGGA_t gpsMessageGGA;

    void defaultConfig();
    void process();
};

} //namespace habsat::gps

#endif  // HABSAT_OBC_GPS_H
