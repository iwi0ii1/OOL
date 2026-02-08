#ifndef FS_FILE_HPP
#define FS_FILE_HPP
#endif

#include <filesystem>
#include <string>
#include <string_view>

namespace alt {
    namespace fs {
        
        namespace sfs = std::filesystem; // Alias for std::filesystem

        // A wrapper around a file
        class file final {
        private:
            std::string name_;
            size_t byte_size_; // By byte (not KB)
            mode_t permission_; // By octal
            sfs::file_time_type lwt_; // Last write time

        public:
            // Wraps around a file (create if not present)
            explicit file(const std::string_view name) noexcept : name_(name) {
                sfs::
            }

        };
    }
}