//
// Created by Jakub Zygmunt on 21.12.2020.
//

template <size_t PathLength>
auto SDReader::fileStatus(std::array<char, PathLength>& path) -> FRESULT {
  return f_stat(path.data(), nullptr);
}

template <size_t PathLength>
auto SDReader::write(std::array<char, PathLength>& path, char* content) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  fresult = file.open(path.data(),FA_CREATE_ALWAYS | FA_WRITE);

  if (fresult == FR_OK) {
    Terminal::pcTransmitDMA("File successfully created");
    fresult = f_write(file.file(), content, strlen(content), nullptr);
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

template <size_t PathLength>
auto SDReader::update(std::array<char, PathLength>& path, char* content) -> FRESULT {
  FRESULT fresult;
  SmartFile file{};

  fresult = fileStatus(path);
  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  fresult = file.open(path.data(),FA_OPEN_APPEND | FA_WRITE);

  if (fresult == FR_OK) {
    fresult = f_write(file.file(), content, strlen(content), nullptr);
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

template <size_t PathLength>
auto SDReader::makeDirectory(std::array<char, PathLength>& path) -> FRESULT {
  FRESULT fresult;

  fresult = f_mkdir(path.data());

  if(fresult != FR_OK){
    // TODO send comm to console
    return fresult;
  }

  return fresult;
  // TODO make void and send comm to console ?
}

template <size_t PathLength>
auto SDReader::remove(std::array<char, PathLength>& path) -> FRESULT {
  FRESULT fresult;

  fresult = fileStatus(path);

  if(fresult != FR_OK){
    // TODO send comm to console (determine if comm should be sent here or in fileStatus)
    return fresult;
  }

  return f_unlink(path.data());
  // TODO make void and send comm to console ?
}

template <size_t PathLength>
auto SDReader::makeFile(std::array<char, PathLength>& path) -> FRESULT {
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