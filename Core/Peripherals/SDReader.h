//
// Created by Jakub Zygmunt on 02.11.2020.
//

#ifndef RCC_SYS_MICROSD_READER_H
#define RCC_SYS_MICROSD_READER_H

#include "fatfs.h"
#include "osTask.h"
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
  // not sure if handle is needed here
  explicit SDReader(SD_HandleTypeDef* sdHandle);
  void init();

  static auto mount() -> FRESULT;
  static auto unmount() -> FRESULT;
  auto format() -> FRESULT;
  auto fileStatus(std::array<char, 256>& path) ->FRESULT;
  auto write(std::array<char, 256>& path, std::array<char, 256>& text) -> FRESULT;
  auto update(std::array<char, 256>& path, std::array<char, 256>& text) -> FRESULT;
  auto remove(std::array<char, 256>& path) -> FRESULT;
  auto make_directory(std::array<char, 256>& path) -> FRESULT;

  void check_free_space();
  static void mmc_task_function(void* args);

 private:
  const os::Task sdReaderTask{"mmc_task", 512, os::Priority::Idle, mmc_task_function};
  SD_HandleTypeDef* m_hsd;

  UINT bytes_written, bytes_read;

  uint32_t total_space, free_space;
  FATFS* pfs;
  DWORD free_clusters;

  DIR directory;
};
}
#endif  // RCC_SYS_MICROSD_READER_H
