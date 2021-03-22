
template<size_t PathLength>
void SDSave::prepareFileHeader(std::array<char, PathLength>&buffer) {
  std::sprintf(buffer.data(), "DD/MM/YY hh:mm:ss Temp(\u2103) Press(hPa) Hum(%%RH)\r\n");
}

template<size_t PathLength>
void SDSave::prepareRTCData(std::array<char, PathLength>&buffer, const hw::Rtc::Buffer* buf) {
  std::sprintf(buffer.data(), "%'.02d/%'.02d/%'.02d %'.02d:%'.02d:%'.02d", buf->day,
               buf->month, buf->year, buf->hour, buf->minute, buf->second);
}

template<size_t PathLength>
void SDSave::prepareSensorData(std::array<char, PathLength>&buffer, const hw::Sensor::Buffer* buf) {
  std::sprintf(buffer.data(), "  %.2lf  %.2lf  %.2lf\r\n", buf->temperature, buf->pressure, buf->humidity);
}