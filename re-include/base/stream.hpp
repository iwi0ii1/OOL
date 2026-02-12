#pragma once

#include "./object.hpp"
#include <cstdio>
#include <system_error>
#include <stdexcept>

namespace ool::base {
    #ifdef __unix__

    #include <unistd.h>
    #include <fcntl.h>

    // Wrapper around a `FD`
    // @note Only inherit this class if need safe `read` / `write`.
    class stream : public virtual object {
    protected:
        int fd_;
        const int mode_;

    public:
        stream(int fd) noexcept : fd_(fd), mode_(fcntl(fd_, F_GETFL) & O_ACCMODE) {}
        
        // Read the stream.
        // @note Throws if failed.
        // @warning Check if returned 0 -> EOF, nothing was read
        size_t read(char* buffer, size_t count) const {
            if (mode_ != O_RDONLY && mode_ != O_RDWR)
                throw std::runtime_error("Cannot read from a non-readable stream / FD.");

            const ssize_t n = ::read(fd_, buffer, count);
            if (n < 0)
                throw std::system_error(errno, std::generic_category(), "read failed");

            return static_cast<size_t>(n);
        }

        // Write to the stream.
        // @note Throws if failed.
        // @warning Check if returned 0 -> EOF, nothing was written
        size_t write(const void* data, size_t count) const {
            if (mode_ != O_WRONLY && mode_ != O_RDWR)
                throw std::runtime_error("Cannot write to a non-writable stream / FD.");

            const ssize_t n = ::write(fd_, data, count);
            if (n < 0)
                throw std::system_error(errno, std::generic_category(), "write failed");

            return static_cast<size_t>(n);
        }

    };





    #elif _WIN32



    

    #include <windows.h>

    // Wrapper around a `HANDLE`
    class stream : public virtual object {
    protected:
    
    public:
        
    }

    #endif
}