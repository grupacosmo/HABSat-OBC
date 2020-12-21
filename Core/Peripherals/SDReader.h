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
#include <array>

//TODO docsies
//TODO change some bsp_driver_sd functions

namespace hw {

class SmartFile {
 public:
  SmartFile() = default;

  auto open(char* path, BYTE mode) {
    return f_open(sdFile_, path, mode);
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

  template<size_t PathLength>
  static auto fileStatus(std::array<char, PathLength>& path) ->FRESULT;

  template<size_t PathLength>
  auto write(std::array<char, PathLength>& path, char* content) -> FRESULT;

  template<size_t PathLength>
  auto update(std::array<char, PathLength>& path, char* content) -> FRESULT;

  template<size_t PathLength>
  static auto remove(std::array<char, PathLength>& path) -> FRESULT;

  template<size_t PathLength>
  static auto makeDirectory(std::array<char, PathLength>& path) -> FRESULT;

  template<size_t PathLength>
  auto makeFile(std::array<char, PathLength>& path) -> FRESULT;

 private:
  const SDIOBus* sdio_{};
};

#include "SDReader.ipp"

}  // namespace hw
#endif  // RCC_SYS_MICROSD_READER_H
