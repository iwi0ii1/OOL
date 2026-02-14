#ifndef FS_FILE_HPP
#define FS_FILE_HPP

#include "../types/object.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace asl::fs {
        
    namespace sfs = std::filesystem; // Alias for std::filesystem

    enum memory_unit { B = 1, KB = 1'000, MB = 1'000'000, GB = 1'000'000'000 };

    // A wrapper around a file
    class file final : private types::object {
    private:
        sfs::path path_;

    public:

        #pragma region Setup

        // Wraps around a file (create if not present or is a directory)
        explicit file(const sfs::path name) : path_(name) {
            const sfs::path p(name);
            if (!sfs::exists(p) || !sfs::is_regular_file(p)) {
                std::ifstream that_file(p.string());
                if (!that_file.is_open()) throw std::runtime_error("asl::fs::file::file(): failed to create file.");
            }
        }

        // Default ctor
        file() = default;

        // Just destructor, lol
        ~file() = default;


        basl operator==(const file& b) const noexcept {
            return this->path() == b.path();
        }

            
        #pragma endregion
            


        #pragma region Info

        // Base name
        sfs::path name() const noexcept {
            return path_.filename();
        }

        // Full path
        const sfs::path& path() const noexcept {
            return path_;
        }

        // The size of the file (default unit: KB)
        std::uintmax_t size(const memory_unit unit = KB) const noexcept {
            return sfs::file_size(path_) / unit;
        }

        // The permission of the file
        sfs::perms permission() const noexcept {
            return sfs::status(path_).permissions();
        }

        // The last modification date
        sfs::file_time_type last_write_time() const noexcept {
            return sfs::last_write_time(path_);
        }

        #pragma endregion
    };
}

#endif