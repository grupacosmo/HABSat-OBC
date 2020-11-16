//
// Created by Jakub Zygmunt on 02.11.2020.
//

#ifndef RCC_SYS_MSDREADER_H
#define RCC_SYS_MSDREADER_H

#include "fatfs.h"
#include "os_task.h"

class MsdReader{

public:

    MsdReader(uint8_t retSD, char *SDPath, FATFS SDFatFS, FIL SDFile);
    static void init();
    FRESULT mount();


private:
    const os::Task rtc_task{"msd_task", 128, os::Task::Priority::IDLE, msd_task_code};

    uint8_t retSD;    /* Return value for SD */
    char SDPath[4];   /* SD logical drive path */
    FATFS SDFatFS;    /* File system object for SD logical drive */
    FIL SDFile;       /* File object for SD */
};

#endif//RCC_SYS_MSDREADER_H
