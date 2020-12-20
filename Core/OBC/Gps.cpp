//
// Created by Wiktor Wieclaw on 12/20/20.
//

#include "Gps.h"

#include <cstdio>
#include <cstring>

struct gps_state gps_init(UART_HandleTypeDef * uart) {
  struct gps_state state;

  state.uart = uart;
  for(uint8_t i=0; i<100; i++) state.line_buffer[i] = '\0';
  state.writer_position = 0;
  state.reader_position = 0;
  for(uint8_t i=0; i<30; i++) state.field_buffer[i] = '\0';
  state.field_position = 0;

  state.date_day = 0;
  state.date_mounth = 0;
  state.date_year = 0;
  state.time_hour = 0;
  state.time_min = 0;
  state.time_sec = 0;

  state.latitude = 0.0;
  state.latitude_direction = '?';
  state.longitude = 0.0;
  state.longitude_direction = '?';
  state.altitude = 0.0;

  state.speed_knots = 0.0;
  state.speed_kilometers = 0.0;

  state.satelites_number = 0;
  state.quality = '?';
  state.dop = 0.0;
  state.hdop = 0.0;
  state.vdop = 0.0;

  return state;
}

void gps_recv_char(struct gps_state * state, uint8_t recv_char) {
  if (state->writer_position == 0 && recv_char == '$') {
    state->writer_position++;
  } else if (state->writer_position >= 1 && state->writer_position < 99) {
    if (recv_char == '\r' || recv_char == '\n') {
      state->line_buffer[state->writer_position - 1] = '\0';
      state->writer_position = 0;
      gps_process_line(state);
    } else {
      state->line_buffer[state->writer_position - 1] = recv_char;
      state->writer_position++;
    }
  } else {
    state->writer_position = 0;
  }
}

void gps_read_field(struct gps_state * state) {
  state->field_position = 0;
  while(state->line_buffer[state->reader_position] != ',' && state->line_buffer[state->reader_position] != '\0'
        && state->field_position < 29) {
    state->field_buffer[state->field_position] = state->line_buffer[state->reader_position];
    state->reader_position++;
    state->field_position++;
  }
  state->field_buffer[state->field_position] = '\0';
  state->reader_position++;
}

void gps_process_line(struct gps_state * state) {
  state->reader_position = 0;
  gps_read_field(state);
  if(strcmp(reinterpret_cast<const char*>(state->field_buffer), "GPRMC") == 0) gps_process_gprmc(state);
  else if(strcmp(reinterpret_cast<const char*>(state->field_buffer), "GPVTG") == 0) gps_process_gpvtg(state);
  else if(strcmp(reinterpret_cast<const char*>(state->field_buffer), "GPGGA") == 0) gps_process_gpgga(state);
  else if(strcmp(reinterpret_cast<const char*>(state->field_buffer), "GPGSA") == 0) gps_process_gpgsa(state);
}

void gps_process_gprmc(struct gps_state * state)
{
  //$GPRMC,212846.00,A,5025.81511,N,01639.92090,E,0.196,,140417,,,A*73
  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) {
    uint32_t tmp;
    sscanf(reinterpret_cast<const char*>(state->field_buffer), "%d", &tmp);
    state->time_sec = tmp % 100;
    state->time_min = (tmp / 100) % 100;
    state->time_hour = (tmp / 10000) % 100;
  }

  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->latitude));

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%c", &(state->latitude_direction));

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->longitude));

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%c", &(state->longitude_direction));

  gps_read_field(state);
  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) {
    uint32_t tmp;
    sscanf(reinterpret_cast<const char*>(state->field_buffer), "%d", &tmp);
    state->date_year = tmp % 100;
    state->date_mounth = (tmp / 100) % 100;
    state->date_day = (tmp / 10000) % 100;
  }
}

void gps_process_gpvtg(struct gps_state * state)
{
  //$GPVTG,,T,,M,0.196,N,0.363,K,A*2B
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->speed_knots));

  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->speed_kilometers));
}

void gps_process_gpgga(struct gps_state * state)
{
  //$GPGGA,212846.00,5025.81511,N,01639.92090,E,1,04,4.72,281.1,M,42.0,M,,*5F
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%d", &(state->quality));

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%d", &(state->satelites_number));

  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->altitude));
}

void gps_process_gpgsa(struct gps_state * state)
{
  //$GPGSA,A,3,10,18,21,15,,,,,,,,,6.79,4.72,4.89*01
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);
  gps_read_field(state);

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->dop));

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->hdop));

  gps_read_field(state);
  if(strlen(reinterpret_cast<const char*>(state->field_buffer)) > 0) sscanf(reinterpret_cast<const char*>(state->field_buffer), "%lf", &(state->vdop));
}