//
// Created by Jakub Zygmunt on 02.11.2020.
//

#ifndef RCC_SYS_MICROSD_READER_H
#define RCC_SYS_MICROSD_READER_H

#include "fatfs.h"
#include "osTask.h"
#include "SDIOBus.h"
#include "Terminal.h"

#include <cstring>
#include <string_view>

//TODO docsies
//TODO change some bsp_driver_sd functions

namespace hw {

class SmartFile {
 public:
  SmartFile() = default;

  auto open(std::string_view path, BYTE mode) {
    return f_open(sdFile_, path.data(), mode);
  }

  auto close(){
    return f_close(sdFile_);
  }

  auto file() -> FIL* {
    return sdFile_;
  }

  ~SmartFile() {
    f_close(sdFile_);
  }

 private:
  FIL* sdFile_ = &SDFile;
};

class SDReader : public Noncopyable{
 public:
  explicit SDReader(const SDIOBus* sdio);
  static void init();
  static auto mount() -> FRESULT;
  static auto unmount() -> FRESULT;
  static auto format() -> FRESULT;
  static void checkFreeSpace();
  static auto fileStatus(std::string_view path) ->FRESULT;
  auto write(std::string_view path, std::string_view content) -> FRESULT;
  auto update(std::string_view path, std::string_view content) -> FRESULT;
  static auto remove(std::string_view path) -> FRESULT;
  static auto makeDirectory(std::string_view path) -> FRESULT;
  auto makeFile(std::string_view path) -> FRESULT;

 private:
  const SDIOBus* sdio_{};
};

}  // namespace hw
#endif  // RCC_SYS_MICROSD_READER_H
