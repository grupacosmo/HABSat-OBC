//
// Created by Jakub Zygmunt on 02.11.2020.
//

#include "SDReader.h"

namespace hw {

SDReader::SDReader(const SDIOBus* sdio) {}

void SDReader::init() {
  BSP_SD_Init();
}

auto SDReader::mount() -> FRESULT {
  return f_mount(&SDFatFS, SDPath, 0);
  // TODO make void and send comm to console ?
}

auto SDReader::unmount() -> FRESULT {
  return f_mount(nullptr, SDPath, 0);
  // TODO make void and send comm to console ?
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
  // TODO make void and send comm to console
}

auto SDReader::fileStatus(std::string_view  path) -> FRESULT {
  return f_stat(path.data(), nullptr);
}

auto SDReader::write(std::string_view path, std::string_view content) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  fresult = file.open(path.data(), FA_CREATE_ALWAYS | FA_WRITE);

  if (fresult == FR_OK) {
    Terminal::pcTransmitDMA("File successfully created");
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
  // TODO make void and send comm to console ?
}

auto SDReader::update(std::string_view path, std::string_view content) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
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
  // TODO make void and send comm to console ?
}

auto SDReader::makeDirectory(std::string_view path) -> FRESULT {
  FRESULT fresult;

  fresult = f_mkdir(path.data());

  if(fresult != FR_OK){
    // TODO send comm to console
    return fresult;
  }

  return fresult;
  // TODO make void and send comm to console ?
}

auto SDReader::remove(std::string_view path) -> FRESULT {
  FRESULT fresult;

  fresult = fileStatus(path);

  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  return f_unlink(path.data());
  // TODO make void and send comm to console ?
}

auto SDReader::makeFile(std::string_view  path) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult == FR_OK){
    //file exist
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  fresult = file.open(path.data(), FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
  if(fresult != FR_OK){
    // TODO send comm to console
    return fresult;
  }

  file.close();

  return fresult;
  // TODO make void and send comm to console ?
}

}  // namespace hw