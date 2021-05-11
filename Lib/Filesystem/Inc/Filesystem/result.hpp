#ifndef HABSAT_OBC_FILESYSTEM_RESULT_HPP
#define HABSAT_OBC_FILESYSTEM_RESULT_HPP

namespace habsat::filesystem {

/// Error codes for functions in filesystem namespace.
enum class Result {
    Ok = 0,            ///< (0) Succeeded
    DiskErr,           ///< (1) A hard error occurred in the low level disk I/O layer
    IntErr,            ///< (2) Assertion failed
    NotReady,          ///< (3) The physical drive cannot work
    NoFile,            ///< (4) Could not find the file
    NoPath,            ///< (5) Could not find the path
    InvalidName,       ///< (6) The path name format is invalid
    Denied,            ///< (7) Access denied due to prohibited access or directory full
    Exist,             ///< (8) Access denied due to prohibited access
    InvalidObject,     ///< (9) The file/directory object is invalid
    WriteProtected,    ///< (10) The physical drive is write protected
    InvalidDrive,      ///< (11) The logical drive number is invalid
    NotEnables,        ///< (12) The volume has no work area
    NoFilesystem,      ///< (13) There is no valid FAT volume
    MkfsAborted,       ///< (14) The f_mkfs() aborted due to any problem
    Timeout,           ///< (15) Could not get a grant to access the volume within defined period
    Locked,            ///< (16) The operation is rejected according to the file sharing policy
    NotEnoughCore,     ///< (17) LFN working buffer could not be allocated
    TooManyOpenFiles,  ///< (18) Number of open files > _FS_LOCK
    InvalidParameter   ///< (19) Given parameter is invalid
};

}  // namespace habsat::filesystem

#endif  // HABSAT_OBC_FILESYSTEM_RESULT_HPP
