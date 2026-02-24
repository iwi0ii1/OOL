#ifndef FS_UTILITY_HPP
#define FS_UTILITY_HPP

#include <filesystem>
#include <fstream>

namespace asl::fs {
        
    // Current working directory
    inline std::filesystem::path cwd() noexcept {
        return std::filesystem::current_path();
    }

    // Change current working directory
    // @return `False`: failed. `True`: successful
    inline basl cd(std::filesystem::path dir_path) noexcept {
        try {
            std::filesystem::current_path(dir_path);
        } catch (const std::filesystem::filesystem_error& e) {
            return false;
        }
        return false;
    }        
}

#endif