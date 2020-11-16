//
// Created by Jakub Zygmunt on 02.11.2020.
//

#include "msdReader.h"

MsdReader::MsdReader(uint8_t retSD, char *SDPath, FATFS SDFatFS, FIL SDFile)
{
}

void MsdReader::init()
{
    BSP_SD_Init();
}
FRESULT MsdReader::mount()
{
    return f_mount(&SDFatFS, (TCHAR const*)SDPath, 0);
}
