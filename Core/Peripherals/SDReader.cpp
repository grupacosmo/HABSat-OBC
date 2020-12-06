//
// Created by Jakub Zygmunt on 02.11.2020.
//

#include "SDReader.h"

#include "Obc.h"

namespace hw {

SDReader::SDReader(SD_HandleTypeDef* sdHandle) : m_hsd(sdHandle) {}

void SDReader::init() {
  BSP_SD_Init();
  sdReaderTask.addToScheduler();
}

auto SDReader::mount() -> FRESULT {
  return f_mount(&SDFatFS, SDPath, 0);
  // TODO make void and send comm to console
}

auto SDReader::unmount() -> FRESULT {
  return f_mount(nullptr, SDPath, 0);
  // TODO make void and send comm to console
}

auto SDReader::fileStatus(std::array<char, 256>& path) -> FRESULT {
  FILINFO fileInfo;
  return f_stat(path.data(), &fileInfo);
}

auto SDReader::write(std::array<char, 256>& path, std::array<char, 256>& text) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  fresult = file.open(path.data(),FA_CREATE_ALWAYS | FA_WRITE);

  if (fresult == FR_OK) {
    fresult = f_write(file.file(), text.data(), text.size(), &bytes_written);
    if (fresult != FR_OK) {
      // TODO send comm to console
    }
  }
  else{
    // TODO send comm to console
  }

  return fresult;
  // TODO make void and send comm to console
}

auto SDReader::update(std::array<char, 256>& path, std::array<char, 256>& text) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  fresult = file.open(path.data(),FA_OPEN_APPEND | FA_WRITE);

  if (fresult == FR_OK) {
    fresult = f_write(file.file(), text.data(), text.size(), &bytes_written);
    if (fresult != FR_OK) {
      // TODO send comm to console
    }
  }
  else{
    // TODO send comm to console
  }

  return fresult;
  // TODO make void and send comm to console
}

auto SDReader::make_directory(std::array<char, 256>& path) -> FRESULT {
  FRESULT fresult;

  fresult = f_mkdir(path.data());

  if(fresult != FR_OK){
    // TODO send comm to console
    return fresult;
  }

  return fresult;
  // TODO make void and send comm to console
}

auto SDReader::remove(std::array<char, 256>& path) -> FRESULT {
  FRESULT fresult;

  fresult = fileStatus(path);

  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  return f_unlink(path.data());
  // TODO make void and send comm to console
}

void SDReader::check_free_space() {
  f_getfree("", &free_clusters, &pfs);
  total_space = static_cast<uint32_t>((pfs->n_fatent - 2) * pfs->csize * 0.5);
  free_space  = static_cast<uint32_t>(free_clusters * pfs->csize * 0.5);
  // TODO send comm to console
}

auto SDReader::format() -> FRESULT {
  FRESULT fresult;
  FILINFO fileInfo;

  fresult = f_opendir(&directory, "/");
  if (fresult == FR_OK) {
    while (true) {
      fresult = f_readdir(&directory, &fileInfo);
      if (fresult != FR_OK || fileInfo.fname[0] == 0) break;

      if (fileInfo.fattrib & AM_DIR && "SYSTEM~1" != fileInfo.fname) {
        if (f_unlink(fileInfo.fname) == FR_DENIED) continue;
      }

      else
        f_unlink(fileInfo.fname);
    }
    f_closedir(&directory);
  }

  return fresult;
  // TODO make void and send comm to console
}

void SDReader::mmc_task_function(void* args) {
  //SDReader& microsd_reader = Obc.peripherals.microsd_reader;
  std::array<char, 256> test_data{"just a sample text"};
  std::array<char, 256> test_path{"test.txt"};

  if (microsd_reader.mount() != FR_OK) {
    os::Task::suspendItself();
  }
  // microsd_reader.format();
  microsd_reader.write(test_path, test_data);

  while (true) {
    microsd_reader.update(test_path, test_data);
    os::Task::delay(1000);
  }
}

}