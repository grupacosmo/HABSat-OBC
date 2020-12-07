//
// Created by Jakub Zygmunt on 02.11.2020.
//

#ifndef RCC_SYS_MICROSD_READER_H
#define RCC_SYS_MICROSD_READER_H

#include "fatfs.h"
#include "osTask.h"
#include "SDIOBus.h"
#include <array>

//TODO docsies
//TODO change some bsp_driver_sd functions

namespace hw {

class SmartFile {
 public:
  SmartFile() = default;

  auto open(char* path, BYTE mode) {
    return f_open(&sdFile_, path, mode);
  }

  auto file() -> FIL* {
    return &sdFile_;
  }

  ~SmartFile() {
    f_close(&sdFile_);
  }

 private:
  FIL sdFile_;
};

class SDReader : public Noncopyable{
 public:
  explicit SDReader(const SDIOBus* sdio);
  static void init();

  static auto mount() -> FRESULT;
  static auto unmount() -> FRESULT;
  static auto format() -> FRESULT;
  static auto fileStatus(std::array<char, 256>& path) ->FRESULT;
  auto write(std::array<char, 256>& path, std::array<char, 256>& text) -> FRESULT;
  auto update(std::array<char, 256>& path, std::array<char, 256>& text) -> FRESULT;
  static auto remove(std::array<char, 256>& path) -> FRESULT;
  static auto makeDirectory(std::array<char, 256>& path) -> FRESULT;
  static auto makeFile(std::array<char, 256>& path) -> FRESULT;

  static void checkFreeSpace();

 private:
  const SDIOBus* sdio_;

  UINT bytes_written, bytes_read;
};

}  // namespace hw
#endif  // RCC_SYS_MICROSD_READER_H
