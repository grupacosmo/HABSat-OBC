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
#include <string_view>

//TODO docsies
//TODO change some bsp_driver_sd functions

namespace hw {

/**
 * Auto-closing handle to wrap FATFS file pointer
 */
class SmartFile {
 public:
  SmartFile() = default;

  auto open(std::string_view path, BYTE mode) {
    return f_open(sdFile_, path.data(), mode);
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
  /**
    * SDReader's constructor.
    * @param sdio
    */
  explicit SDReader(const SDIOBus* sdio);

  /**
    * Initializes SDReader/SDCard.
    */
  static void init();

  /**
    * Mounts SDCard.
    * @return File function return code (FRESULT).
    */
  static auto mount() -> FRESULT;

  /**
    * Unmounts SDCard.
    * @return File function return code (FRESULT).
    */
  static auto unmount() -> FRESULT;

  /**
    * Formats the mounted SDCard. Unlinks all the possible files.
    * @return File function return code (FRESULT).
    */
  static auto format() -> FRESULT;

  /**
    * Checks the SDCard memory usage and prints it in the terminal.
    */
  static void checkFreeSpace();

  /**
   * Checks if the file exists.
   * @param path        file path
   * @return File function return code (FRESULT).
   */
  static auto fileStatus(std::string_view path) ->FRESULT;

  /**
   * Creates new file and writes the data (content) to the file.
   * @param path        file path
   * @param content     data to write
   * @return File function return code (FRESULT).
   */
  auto write(std::string_view path, std::string_view content) -> FRESULT;

  /**
   * Updates the file content by writing the data (content) at its end.
   * @param path        file path
   * @param content     data to write
   * @return File function return code (FRESULT).
   */
  auto update(std::string_view path, std::string_view content) -> FRESULT;

  /**
   * Unlinks the given file
   * @param path        file path
   * @return File function return code (FRESULT).
   */
  static auto remove(std::string_view path) -> FRESULT;

  /**
     * Creates a directory
    * @param path        directory path
    * @return File function return code (FRESULT).
    */
  static auto makeDirectory(std::string_view path) -> FRESULT;

  /**
    * Creates an empty file
    * @param path        directory path
    * @return File function return code (FRESULT).
    */
  auto makeFile(std::string_view path) -> FRESULT;

 private:
  const SDIOBus* sdio_{};
};

}  // namespace hw
#endif  // RCC_SYS_MICROSD_READER_H
