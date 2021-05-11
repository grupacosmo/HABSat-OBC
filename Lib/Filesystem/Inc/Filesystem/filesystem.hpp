#ifndef HABSAT_OBC_FILESYSTEM_FILESYSTEM_HPP
#define HABSAT_OBC_FILESYSTEM_FILESYSTEM_HPP

#include "Utils/to_underlying.hpp"
#include "result.hpp"

/// C++ wrapper for FatFs.
/// See FatFs for details http://elm-chan.org/fsw/ff/00index_e.html
namespace habsat::filesystem {

/// Specifies the sd card mounting mode.
enum class MountingMode : std::uint8_t {
    Delayed = 0,  ///< Mount at the first use
    Immediate     ///< Mount immediately
};

/// Mounts the SD card.
/// \param sdFatFs Filesystem object structure (FATFS)
/// \param sdPath SD logical drive path
[[nodiscard("handle errors")]] auto mount(FATFS& sdFatFs, char (&sdPath)[4], MountingMode mode)
      -> Result {
    return static_cast<Result>(f_mount(&sdFatFs, sdPath, utils::toUnderlying(mode)));
}

/// Unmounts the SD card.
/// \param sdPath SD logical drive path
[[nodiscard("handle errors")]] auto unmount(char (&sdPath)[4]) -> Result {
    return static_cast<Result>(f_mount(nullptr, sdPath, 0));
}

}  // namespace habsat::filesystem

#endif  // HABSAT_OBC_FILESYSTEM_FILESYSTEM_HPP
