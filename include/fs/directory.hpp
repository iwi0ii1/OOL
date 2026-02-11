#ifndef FS_DIRECTORY_HPP
#define FS_DIRECTORY_HPP

#include "../types/object.hpp"
#include "./file.hpp"
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>


namespace ool::fs {

    // A wrapper around a directory
    class directory final : private types::object {
    private:
        sfs::path path_;

        std::vector<sfs::path> children_;

    public:

        #pragma region Setups

        // Wraps around a directory (create if not present or is a regular file)
        explicit directory(const sfs::path name) noexcept : path_(name) {
            const sfs::path& p = name;

            if (sfs::exists(p) && sfs::is_directory(p)) {
                // Emplace children
                for (const auto& entry : sfs::recursive_directory_iterator(p))
                    children_.push_back(entry.path());
                    
            } else {
                sfs::create_directory(p); // Create directory
            }
        }

        // Default ctor
        directory() = default;

        // Just destructor, lol
        ~directory() = default;


        bool operator==(const directory& b) const noexcept {
            return this->path() == b.path();
        }


        #pragma endregion





        #pragma region Info

        // Base name
        sfs::path name() const noexcept {
            return path_.filename();
        }

        // The full path of the directory
        const sfs::path& path() const noexcept  {
            return path_;
        }

        // The size of the directory (default unit: KB)
        std::uintmax_t size(const memory_unit unit = KB) const noexcept {
            std::uintmax_t byte_size_ = 0;
            for (const auto& each : sfs::recursive_directory_iterator(path_))
                if (sfs::is_regular_file(each.path())) byte_size_ += each.file_size();

            return byte_size_ / unit;
        }

        // The permission of the directory
        sfs::perms permission() const noexcept {
            return sfs::status(path_).permissions();
        }

        // The last modification date
        sfs::file_time_type last_write_time() const noexcept {
            return sfs::last_write_time(path_);
        }

        // Children
        const auto& list() const noexcept {
            return children_;
        }

        #pragma endregion




        #pragma region Modifier

        // Create new file
        directory& new_file(const sfs::path& file_path) {
            if (sfs::exists(file_path))
                throw std::runtime_error("ool::fs::directory::new_file(): Given path already existed.");

            std::ofstream newer(file_path.string());
            if (!newer.is_open())
                throw std::runtime_error("ool::fs::directory::new_file(): Failed to create file.");
                
            children_.emplace_back(file_path);
            return *this;
        }

        // Create a new directory
        directory& new_directory(const sfs::path& dir_path) {
            if (sfs::exists(dir_path))
                throw std::runtime_error("ool::fs::directory::new_directory(): Given path already existed.");

            if (!sfs::create_directory(dir_path))
                throw std::runtime_error("ool::fs::directory::new_directory(): Failed to create directory.");

            children_.emplace_back(dir_path);
            return *this;
        }
            
        // Remove a file / directory
        // @warning Destructive. Aggressive.
        directory& remove_child(const sfs::path& path) {
            if (!sfs::exists(path))
                throw std::runtime_error("ool::fs::directory::remove_child(): Given path does not exist.");

            const auto it = std::find(children_.begin(), children_.end(), path);

            if (it == children_.end())
                throw std::runtime_error("ool::fs::directory::remove_child(): Given path doesn't seem to exist in internal list (might be a bug).");

            if (!sfs::remove_all(path))
                throw std::runtime_error("ool::fs::directory::remove_child(): Failed to remove child.");

            children_.erase(it);
        }
    };
}

#endif