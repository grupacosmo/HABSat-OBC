//
// Created by Natalia Bidzinska on 15.02.2021.
//

#include "Gps.h"

#include "cstring"

hw::Gps::Gps(UART_HandleTypeDef* uartHandle) : uartHandle(uartHandle) {}

void hw::Gps::init() const {
  // TODO
}

void hw::Gps::recvChar(uint8_t recv_char) {
  if (buffer.writerPosition == 0 && recv_char == '$') {
    buffer.writerPosition++;
  } else if (buffer.writerPosition >= 1 && buffer.writerPosition < 99) {
    if (recv_char == '\r' || recv_char == '\n') {
      buffer.lineBuffer[buffer.writerPosition - 1] = '\0';
      buffer.writerPosition                        = 0;
      processLine();
    } else {
      buffer.lineBuffer[buffer.writerPosition - 1] = recv_char;
      buffer.writerPosition++;
    }
  } else {
    buffer.writerPosition = 0;
  }
}

void hw::Gps::readField() {
  buffer.fieldPosition = 0;
  while (buffer.lineBuffer[buffer.readerPosition] != ',' &&
         buffer.lineBuffer[buffer.readerPosition] != '\0' && buffer.fieldPosition < 29) {
    buffer.fieldBuffer[buffer.fieldPosition] = buffer.lineBuffer[buffer.readerPosition];
    buffer.readerPosition++;
    buffer.fieldPosition++;
  }
  buffer.fieldBuffer[buffer.fieldPosition] = '\0';
  buffer.readerPosition++;
}

void hw::Gps::processLine() {
  // TODO
  buffer.readerPosition = 0;
  readField();
  if (strcmp(buffer.fieldBuffer, "GPRMC") == 0)
    processGprmc();
  else if (strcmp(buffer.fieldBuffer, "GPGGA") == 0)
    processGpgga();
  else if (strcmp(buffer.fieldBuffer, "GPVTG") == 0)
    processGpvtg();
  else if (strcmp(buffer.fieldBuffer, "GPGSA") == 0)
    processGpgsa();
}

void hw::Gps::processGprmc() {
  /*
   * $GPRMC
   * TIMESTAMP [HHMMSS.SSS]
   * STATUS [A/V] (valid, not valid)
   * LATITUDE [DDMM.MMMMM]
   * LATITUDE [N/S] (north, south)
   * LONGITUDE [DDDMM.MMMMM]
   * LONGITUDE [W/E] (west, east)
   * SPEED OVER GROUND [knots]
   * COURSE OVER GROUND [degrees]
   * DATE [DDMMYY]
   * MAGNETIC VARIATION [degrees] (?)
   * MAGNETIC VARIATION INDICATOR [E/W] (east, west)
   * MODE [A/D/E] (autonomous, DGPS, DR)
   * CHECKSUM (starts with *)
   */
  //$GPRMC,212846.00,A,5025.81511,N,01639.92090,E,0.196,,140417,,,A*73

  // timestamp
  readField();
  if (strlen(buffer.fieldBuffer) > 0) {
    uint32_t tmp;
    sscanf((buffer.fieldBuffer), "%d", &tmp);
    dateTime.sec  = tmp % 100;
    dateTime.min  = (tmp / 100) % 100;
    dateTime.hour = (tmp / 10000) % 100;
  }

  readField();  // status
  if (strlen(buffer.fieldBuffer) > 0)
    sscanf(buffer.fieldBuffer, "%c", &other.statusGprmc);  // TODO: to check

  // latitutde
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &location.latitude);
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%c", &location.latitudeDirection);

  // longitude
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &location.longitude);
  readField();
  if (strlen(buffer.fieldBuffer) > 0)
    sscanf(buffer.fieldBuffer, "%c", &location.longitudeDirection);

  readField();  // speed knots
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &velocity.speedKnots);

  readField();  // course degrees
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &velocity.courseDegrees);

  // date
  readField();
  if (strlen(buffer.fieldBuffer) > 0) {
    uint32_t tmp;
    sscanf(buffer.fieldBuffer, "%d", &tmp);
    dateTime.year  = tmp % 100;
    dateTime.month = (tmp / 100) % 100;
    dateTime.day   = (tmp / 10000) % 100;
  }
}

void hw::Gps::processGpgga() {
  /*
   * $GPGGA
   * TIMESTAMP [HHMMSS.SSS]
   * LATITUDE [DDMM.MMMMM]
   * LATITUDE [N/S] (north, south)
   * LONGITUDE [DDDMM.MMMMM]
   * LONGITUDE [W/E] (west, east)
   * QUALITY INDICATOR
   *    - 1 = Uncorrected coordinate
   *    - 2 = Differentially correct coordinate
   *    - 4 = RTK Fix coordinate (centimeter precision)
   *    - 5 = RTK Float (decimeter precision)
   * NUMBER OF SATELLITES
   * HDOP (horizontal dilution of precision (?)
   * ALTITUDE OF THE ANTENNA
   * ALTITUDE UNITS [M/F] (meters, feet)
   * GEOIDAL SEPARATION (?)
   * GEOIDAL SEPARATION UNITS
   * AGE OF CORRECTION (?)
   * CORRECTION STATION ID (?)
   * CHECKSUM (starts with *)
   */
  //$GPGGA,212846.00,5025.81511,N,01639.92090,E,1,04,4.72,281.1,M,42.0,M,,*5F

  // TODO: update timestamp and location here?
  readField();  // timestamp
  readField();  // latitude
  readField();  // latitude direction
  readField();  // lonigtude
  readField();  // longitude direction

  // quality
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%s", &other.qualityGpgga);

  // satellites number
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%s", &other.satelitesNumber);

  readField();  // hdop

  // altitude of antenna
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &location.altitude);

  // altitude units
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%c", &location.altitudeUnits);
}

void hw::Gps::processGpvtg() {
  /*
   * $GPCTG
   * ? Track made good (degrees true)
   * T
   * ? Track made good (degrees magnetic)
   * M
   * SPEED IN KNOTS
   * N (knots)
   * SPEED IN KILOMETERS PER HOUR [KPH]
   * K (kilometers)
   * CHECKSUM (starts with *)
   */
  //$GPVTG,,T,,M,0.196,N,0.363,K,A*2B
  readField();  // ?
  readField();  // T
  readField();  // ?
  readField();  // M

  // speed in knots
  readField();
  //  if(strlen((state->field_buffer)) > 0) sscanf((state->field_buffer), "%lf",
  //  &(state->speed_knots));

  readField();  // N sign (for knots speed)

  // speed in kmh
  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &velocity.speedKilometers);
}

void hw::Gps::processGpgsa() {
  // to delete?

  //$GPGSA,A,3,10,18,21,15,,,,,,,,,6.79,4.72,4.89*01
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();
  readField();

  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &other.dop);

  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &other.hdop);

  readField();
  if (strlen(buffer.fieldBuffer) > 0) sscanf(buffer.fieldBuffer, "%lf", &other.vdop);
}
