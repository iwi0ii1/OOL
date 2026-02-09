#ifndef FS_DIRECTORY_HPP
#define FS_DIRECTORY_HPP


#include "./file.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <fstream>
#include <filesystem>


inline namespace ool {
    namespace fs {

        // A wrapper around a directory
        class directory final {
        private:
            sfs::path name_;
            std::uintmax_t byte_size_;
            sfs::perms permission_;
            sfs::file_time_type lwt_; // Last write time

            std::vector<file> files_list_;
            std::vector<directory> directories_list_;

        public:
            // Wraps around a directory (create if not present or is a regular file)
            explicit directory(const sfs::path name) noexcept : name_(name), byte_size_(0) {
                const sfs::path& p = name;

                if (!sfs::exists(p) || !sfs::is_directory(p))
                    sfs::create_directory(p);

                for (const auto& entry : sfs::recursive_directory_iterator(p)) {
                    // Sum size
                    if (sfs::is_regular_file(entry.path()))
                        byte_size_ += sfs::file_size(entry.path());

                    // Setup directories list
                    if (sfs::is_directory(entry.path()))
                        directories_list_.emplace_back(entry.path());

                    // Setup files list
                    if (sfs::is_regular_file(entry.path()))
                        files_list_.emplace_back(entry.path());
                }

                permission_ = sfs::status(p).permissions();
                lwt_ = sfs::last_write_time(p);
            }

            // Just destructor, lol
            ~directory() = default;





            #pragma region Info

            // Base name
            sfs::path name() const noexcept {
                return name_.filename();
            }

            // The full path of the directory
            const sfs::path& path() const noexcept  {
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




            #pragma region Children

            // List children files
            const std::vector<file>& list_files() const noexcept {
                return files_list_;
            }

            // List children directories
            const std::vector<directory>& list_directories() const noexcept {
                return directories_list_;
            }

            #pragma endregion




            #pragma region Modifier

            // Create new file
            directory& new_file(const sfs::path file_path) {
                files_list_.emplace_back(file_path);
                return *this;
            }

            // Create a new directory
            directory& new_directory(const sfs::path dir_path) {
                directories_list_.emplace_back(dir_path);
                return *this;
            }



            // Remove file
            directory& remove_file(const sfs::path file_path) {
                const auto it = std::find(files_list_.begin(), files_list_.end(), file_path);
                if (it == files_list_.end())
                    throw std::runtime_error("ool::fs::directory::remove_file(): File does not exist. Cannot remove.");

                if (!sfs::remove(it->path()))
                    throw std::runtime_error("ool::fs::directory::remove_file(): Failed to remove file.");

                files_list_.erase(it);
                return *this;
            }

            // Remove directory (destructive, recursive)
            directory& remove_directory(const sfs::path dir_path) {
                const auto it = std::find(directories_list_.begin(), directories_list_.end(), dir_path);
                if (it == directories_list_.end())
                    throw std::runtime_error("ool::fs::directory::remove_directory(): Directory does not exist. Cannot remove.");

                if (!sfs::remove_all(it->path()))
                    throw std::runtime_error("ool::fs::directory::remove_directory(): Faile to remove directory.");

                directories_list_.erase(it);
                return *this;
            }
        };
    }
}

#endif