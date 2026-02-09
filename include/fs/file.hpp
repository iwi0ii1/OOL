#ifndef FS_FILE_HPP
#define FS_FILE_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

inline namespace ool {
    namespace fs {
        
        namespace sfs = std::filesystem; // Alias for std::filesystem

        enum memory_unit { B = 1, KB = 1'000, MB = 1'000'000, GB = 1'000'000'000 };

        // A wrapper around a file
        class file final {
        private:
            sfs::path name_;
            std::uintmax_t byte_size_;
            sfs::perms permission_;
            sfs::file_time_type lwt_; // Last write time

        public:

            #pragma region Setup

            // Wraps around a file (create if not present or is a directory)
            explicit file(const sfs::path name) : name_(name) {
                const sfs::path p(name);
                if (!sfs::exists(p) || !sfs::is_regular_file(p)) {
                    std::ifstream that_file(p.string());
                    if (!that_file.is_open()) throw std::runtime_error("ool::fs::file::file(): failed to create file.");
                }

                byte_size_ = sfs::file_size(p);
                permission_ = sfs::status(p).permissions();
                lwt_ = sfs::last_write_time(p);
            }

            // Default ctor
            file() = default;

            // Just destructor, lol
            ~file() = default;


            bool operator==(const file& b) const noexcept {
                return this->path() == b.path();
            }

            
            #pragma endregion
            


            #pragma region Info

            // Base name
            sfs::path name() const noexcept {
                return name_.filename();
            }

            // Full path
            const sfs::path& path() const noexcept {
                return name_;
            }

            // The size of the directory (default unit: KB)
            std::uintmax_t size(const memory_unit unit = KB) const noexcept {
                return byte_size_ / unit;
            }

            // The permission of the directory
            const sfs::perms& permission() const noexcept {
                return permission_;
            }

            // The last modification date
            const sfs::file_time_type& last_modify_date() const noexcept {
                return lwt_;
            }

            #pragma endregion
        };
    }
}

#endif