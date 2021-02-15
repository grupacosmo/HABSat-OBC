//
// Created by Natalia Bidzinska on 15.02.2021.
//

#ifndef HABSAT_OBC_GPS_H
#define HABSAT_OBC_GPS_H

#include <array>

#include "osTask.h" // TODO: delete?
#include "stm32f4xx.h"

namespace hw {

class Gps {
 public:
  // TODO: default values?
  struct dateTime {
    uint8_t day = 1;
    uint8_t month = 1;
    uint8_t year = 1;
    uint8_t hour = 1;
    uint8_t min = 1;
    uint8_t sec = 1;
  } dateTime;

  struct location {
    double latitude = 0.;
    char latitudeDirection = '?';
    double longitude = 0.;
    char longitudeDirection = '?';
    double altitude = 0.;
    char altitudeUnits = '?';
  } location;

  struct velocity {
    double speedKnots = 0.;
    double speedKilometers = 0.;
    double courseDegrees = 0.;
  } velocity;

  struct other { // TODO: CHANGE NAME! (or delete)
    uint8_t satelitesNumber = 0;
    char statusGprmc = '0';
    uint8_t qualityGpgga = 0;
    double dop = 0.;
    double hdop = 0.;
    double vdop = 0.;
  } other;

 public:
  Gps(UART_HandleTypeDef* uartHandle);
  void init() const;

  void recvChar(uint8_t recv_char);
  void readField();
  void processLine();

 private:
  UART_HandleTypeDef* uartHandle;

  struct buffer { // TODO: change name (variables used to read and write data to structs)
    char lineBuffer[100] = {'\0'}; // TODO: do sprawdzenia
    uint8_t writerPosition = 0;
    uint8_t readerPosition = 0;
    char fieldBuffer[30] = {'\0'}; // TODO: do sprawdzenia
    uint8_t fieldPosition = 0;
  } buffer;

 private:
  void processGprmc(); // date, time, locaiton, speed in knots, course degree
  void processGpgga(); // hour, location, satellites number, altitude
  void processGpvtg(); // TODO: delete? (speed in kmh, 1kmh = 1.85166knots)
  void processGpgsa(); // TODO: delete? (dop, hdop, vdop)
};

}  // namespace hw

#endif  // HABSAT_OBC_GPS_H
