#ifndef HABSAT_OBC_FILESYSTEM_FILE_HPP
#define HABSAT_OBC_FILESYSTEM_FILE_HPP

#include "result.hpp"
#include "Utils/to_underlying.hpp"

namespace habsat::filesystem {

/// SD Card file handle.
/// \warning Mount SD Card before use. See mount()
class File {
   public:
    /// Specifies the file mode. Flags can be combined using "|" operator
    enum class Mode : std::uint8_t {
        Read         = 0x01,  ///< Read access
        Write        = 0x02,  ///< Write access
        OpenExisting = 0x00,  ///< Fails if not existing
        CreateNew    = 0x04,  ///< Fails if already exists
        CreateAlways = 0x08,  ///< Overwrites if file alrady exists
        OpenAlways   = 0x10,  ///< Creates new file only if not existing
        OpenAppend   = 0x30   ///< Same as OpenAlways except that it sets pointer to EOF
    };

    File() = default;

    File(const File& file) = delete;

    File(File&& file) = delete;

    auto operator=(const File& file) -> File& = delete;

    auto operator=(File&& file) -> File& = delete;

    /// Automatically closes the file.
    ~File() {
        static_cast<void>(close());  // cast silences [[nodiscard]]
    }

    [[nodiscard("handle errors")]] auto open(std::string_view path, Mode mode) -> Result {
        return static_cast<Result>(f_open(&handle_, path.data(), utils::toUnderlying(mode)));
    }

    [[nodiscard("handle errors")]] auto read(gsl::span<char> buffer) -> Result {
        return static_cast<Result>(f_read(&handle_, buffer.data(), buffer.size(), nullptr));
    }

    [[nodiscard("handle errors")]] auto write(gsl::span<const char> buffer) -> Result {
        return static_cast<Result>(f_write(&handle_, buffer.data(), buffer.size(), nullptr));
    }

    /// Closes the file.
    /// It is not mandatory to close the file manually as it will be closed in the destructor
    [[nodiscard("handle errors")]] auto close() -> Result {
        return static_cast<Result>(f_close(&handle_));
    }

   private:
    FIL handle_; // cannot be copied since FatFs stores its address
};

/// Allows to combine File::Mode flags.
auto operator|(File::Mode lhs, File::Mode rhs) -> File::Mode {
    return static_cast<File::Mode>(utils::toUnderlying(lhs) | utils::toUnderlying(rhs));
}

}  // namespace habsat::filesystem

#endif  // HABSAT_OBC_FILESYSTEM_FILE_HPP
