//
// Created by Jakub Zygmunt on 02.11.2020.
//

#include "SDReader.h"

#include <array>

namespace hw {

SDReader::SDReader(const SDIOBus* sdio) {}

void SDReader::init() {
  BSP_SD_Init();
}

auto SDReader::mount() -> FRESULT {
  return f_mount(&SDFatFS, SDPath, 0);
}

auto SDReader::unmount() -> FRESULT {
  return f_mount(nullptr, SDPath, 0);
}

auto SDReader::format() -> FRESULT {
  FRESULT fresult;
  FILINFO fileInfo;
  DIR directory;

  fresult = f_opendir(&directory, "/");
  if (fresult == FR_OK) {
    while (true) {
      fresult = f_readdir(&directory, &fileInfo);
      if (fresult != FR_OK || fileInfo.fname[0] == 0) break;

      if (fileInfo.fattrib & AM_DIR && strcmp("SYSTEM~1", fileInfo.fname) != 0) {
        if (f_unlink(fileInfo.fname) == FR_DENIED) continue;
      }

      else
        f_unlink(fileInfo.fname);
    }
    f_closedir(&directory);
  }

  return fresult;
}

auto SDReader::fileStatus(std::string_view  path) -> FRESULT {
  return f_stat(path.data(), nullptr);
}

auto SDReader::write(std::string_view path, std::string_view content) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    return fresult;
  }

  fresult = file.open(path.data(), FA_CREATE_ALWAYS | FA_WRITE);

  if (fresult == FR_OK) {
    fresult = f_write(file.file(), content.data(), content.size(), nullptr);
    if (fresult != FR_OK) {
      return fresult;
      // TODO send comm to console
    }
  }
  else{
    // TODO send comm to console
  }
  return fresult;
}

auto SDReader::update(std::string_view path, std::string_view content) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    return fresult;
  }

  fresult = file.open(path.data(),FA_OPEN_APPEND | FA_WRITE);

  if (fresult == FR_OK) {
    fresult = f_write(file.file(), content.data(), content.size(), nullptr);
    if (fresult != FR_OK) {
      return fresult;
      // TODO send comm to console
    }
    Terminal::pcTransmitDMA("File successfully updated");
  }
  else{
    // TODO send comm to console
  }

  return fresult;
}

auto SDReader::makeDirectory(std::string_view path) -> FRESULT {
  FRESULT fresult;

  fresult = f_mkdir(path.data());

  if(fresult != FR_OK){
    // TODO send comm to console
    return fresult;
  }

  return fresult;
}

auto SDReader::remove(std::string_view path) -> FRESULT {
  FRESULT fresult;

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    return fresult;
  }

  return f_unlink(path.data());
}

auto SDReader::makeFile(std::string_view  path) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult == FR_OK){
    //file exist
    return fresult;
  }

  fresult = file.open(path.data(), FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
  if(fresult != FR_OK){
    // TODO send comm to console
    return fresult;
  }

  file.close();

  return fresult;
}

void SDReader::checkFreeSpace() {
  static DWORD freeClusters;
  static uint32_t totalSpace, freeSpace;
  static FATFS* pfs;
  std::array<char, 100> buffer{};

  f_getfree("", &freeClusters, &pfs);
  totalSpace = static_cast<uint32_t>((pfs->n_fatent - 2) * pfs->csize * 0.5);
  freeSpace  = static_cast<uint32_t>(freeClusters * pfs->csize * 0.5);
  std::sprintf(buffer.data(), "SDCard memory usage: %lu/%lu\n", totalSpace, freeSpace);
  Terminal::pcTransmitDMA(buffer.data());
}

}  // namespace hw