#ifndef FS_FILE_HPP
#define FS_FILE_HPP
#endif

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

namespace alt {
    namespace fs {
        
        namespace sfs = std::filesystem; // Alias for std::filesystem

        // A wrapper around a file
        class file final {
        private:
            std::string name_;
            std::uintmax_t byte_size_;
            sfs::perms permission_;
            sfs::file_time_type lwt_; // Last write time

        public:
            // Wraps around a file (create if not present or is a directory)
            explicit file(const std::string_view name) noexcept : name_(name) {
                const sfs::path p(name);
                if (!sfs::exists(p) || !sfs::is_regular_file(p))
                    std::ifstream that_file(p);

                byte_size_ = sfs::file_size(p);
                permission_ = sfs::status(p).permissions();
                lwt_ = sfs::last_write_time(p);
            }


            // TODO: Implement methods for manipulation
        };
    }
}