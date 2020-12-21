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

}  // namespace hw