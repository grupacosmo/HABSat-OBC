//
// Created by Jakub Zygmunt on 02.11.2020.
//

#ifndef RCC_SYS_MMC_READER_H
#define RCC_SYS_MMC_READER_H

#include "fatfs.h"
#include "os_task.h"
#include <array>

//TODO docsies
//TODO change some bsp_driver_sd functions

class MmcReader
{

public:

    //not sure if handle is needed here
    MmcReader(SD_HandleTypeDef* sdHandle);
    void init();

    FRESULT mount(void);
    FRESULT unmount(void);
    FRESULT format(void);
    FRESULT write(std::array<char, 256> &path, std::array<char, 256> &text);
    FRESULT update(std::array<char, 256> &path, std::array<char, 256> &text);
    FRESULT remove(std::array<char, 256> &path);
    FRESULT make_directory(std::array<char, 256> &path);

    void check_free_space(void);
    static void mmc_task_function(void *args);

private:

    const os::Task mmc_task{"mmc_task", 512, os::Task::Priority::IDLE, mmc_task_function};
    SD_HandleTypeDef *m_hsd;

    UINT bytes_written, bytes_read;

    uint32_t total_space, free_space;
    FATFS *pfs;
    DWORD free_clusters;

    DIR directory;
    FILINFO file_info;

};

#endif//RCC_SYS_MMC_READER_H
