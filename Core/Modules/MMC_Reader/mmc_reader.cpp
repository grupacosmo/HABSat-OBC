//
// Created by Jakub Zygmunt on 02.11.2020.
//

#include "mmc_reader.h"
#include "obc.h"

MmcReader::MmcReader(SD_HandleTypeDef* sdHandle) : m_hsd(sdHandle){}

void MmcReader::init(void){
    BSP_SD_Init();
    mmc_task.add_to_scheduler();

}

FRESULT MmcReader::mount(void){
    return f_mount(&SDFatFS, SDPath, 0);
    //TODO additional lcd or led comm
}

FRESULT MmcReader::unmount(void){
    return f_mount(NULL, SDPath, 0);
    //TODO additional lcd or led comm
}

FRESULT MmcReader::write(std::array<char, 256> &path, std::array<char, 256> &text){
    //TODO check if the file exist in order to not overwrite it
    FRESULT fresult;

    fresult = f_open(&SDFile,path.data(), FA_CREATE_ALWAYS | FA_WRITE);
    if(fresult == FR_OK){

        fresult = f_write(&SDFile, text.data(), text.size(), &bytes_written);
        if(fresult == FR_OK){
            //TODO additional lcd or led comm
        }

        f_close(&SDFile);
    }

    return fresult;
}

FRESULT MmcReader::update(std::array<char, 256> &path, std::array<char, 256> &text){
    //TODO check if the file exist
    FRESULT fresult;

    fresult = f_open(&SDFile, path.data(), FA_OPEN_APPEND | FA_WRITE);
    if(fresult == FR_OK){

        fresult = f_write(&SDFile, text.data(), text.size(), &bytes_written);
        if(fresult == FR_OK){
            //TODO additional lcd or led comm
        }

        f_close(&SDFile);
    }

    return fresult;
}

FRESULT MmcReader::make_directory(std::array<char, 256> &path){
    //TODO check if the dir exist
    return f_mkdir(path.data());
    //TODO additional lcd or led comm
}

FRESULT MmcReader::remove(std::array<char, 256> &path){
    //TODO check if the file exist
    return f_unlink(path.data());
}

void MmcReader::check_free_space(void){
    f_getfree("", &free_clusters, &pfs);
    total_space = static_cast<uint32_t>((pfs->n_fatent - 2) * pfs->csize * 0.5);
    free_space = static_cast<uint32_t>(free_clusters * pfs->csize * 0.5);
    //TODO comm
}

FRESULT MmcReader::format(void){

    std::array<char, 2> path{"/"};
    FRESULT fresult;

    fresult = f_opendir(&directory, path.data());
    if (fresult == FR_OK){

        while (true){

            fresult = f_readdir(&directory, &file_info);
            if (fresult != FR_OK || file_info.fname[0] == 0)
                break;

            if (file_info.fattrib & AM_DIR && "SYSTEM~1" != file_info.fname){
                if (f_unlink(file_info.fname) == FR_DENIED)
                    continue;
            }

            else
                f_unlink(file_info.fname);
        }
        f_closedir(&directory);
    }

    return fresult;
}

void MmcReader::mmc_task_function(void *args){

    MmcReader &mmcReader = obc.peripherals.mmcReader;
    std::array<char, 256> test_data{"just a sample text"};
    std::array<char, 256> test_path{"test.txt"};

    if(mmcReader.mount()!= FR_OK)
    {
        os::Task::suspend_itself();
    }
    //mmcReader.format();
    mmcReader.write(test_path, test_data);

    while (true)
    {
        mmcReader.update(test_path, test_data);
        os::Task::delay(1000);
    }
}