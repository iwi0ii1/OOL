#ifndef FS_DIRECTORY_HPP
#define FS_DIRECTORY_HPP

#include "./file.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <filesystem>

namespace alt {
    namespace fs {

        // A wrapper around a directory
        class directory final {
        private:
            std::string name_;
            std::uintmax_t byte_size_;
            sfs::perms permission_;
            sfs::file_time_type lwt_; // Last write time

            std::vector<file> files_list_;
            std::vector<directory> directories_list_;

        public:
            // Wraps around a directory (create if not present or is a regular file)
            explicit directory(const std::string_view name) noexcept : name_(name) {
                const sfs::path p(name);
                if (!sfs::exists(p) || !sfs::is_directory(p))
                    sfs::create_directory(p);

                for (const auto& entry : sfs::recursive_directory_iterator(p)) {
                    if (sfs::is_regular_file(entry.path())) {
                        byte_size_ += sfs::file_size(entry.path());
                    }

                    if (sfs::is_directory(entry.path()))
                }

                permission_ = sfs::status(p).permissions();
                lwt_ = sfs::last_write_time(p);
            }


            // TODO: Implement methods for manipulation
        };
    }
}

#endif