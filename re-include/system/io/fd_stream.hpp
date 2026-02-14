/*
POSIX only.
FD wrapper.
*/

#pragma once

#ifdef __unix__

#include "../../base/stream.hpp"

#include <system_error>
#include <unistd.h>
#include <fcntl.h>

namespace asl::system::io {
    // File Descriptor wrapper
    // @note Doesn't create file if doesn't exist.
    // @warning Only movable
    class fd_stream final : public base::stream {
    private:
        int fd_;
    public:
        #pragma region Ctor/Dtor/Optor

        // Open file.
        // @note Will throw
        fd_stream(const char* file_path, const int flags) {
            fd_ = open(file_path, flags);
            if (fd_ == -1)
                throw std::system_error(errno, std::generic_category(), std::string{"open() failed for "} + file_path);
            
        }

        fd_stream(const fd_stream&) = delete;
        fd_stream& operator=(const fd_stream&) = delete;


        // Construct current fd by another fd_stream.
        fd_stream(fd_stream&& other) noexcept : fd_(other.fd_) {
            other.fd_ = -1; // Mark invalid.
        }

        // Overwrite current fd.
        fd_stream& operator=(fd_stream&& other) noexcept {
            // Prevent self-move assignment
            if (this != &other) {
                if (fd_ != -1)
                    ::close(fd_);

                fd_ = other.fd_;
                other.fd_ = -1;
            }
            return *this;
        }



        // Close file conditionally
        ~fd_stream() override {
            if (fd_ != -1)
                ::close(fd_);
            fd_ = -1;
        }

        #pragma endregion






        #pragma region Methods

        // The wrapped FD.
        int release() noexcept {
            int temp = fd_;
            fd_ = -1;
            return temp;
        }


        // Read FD
        size_t read(char* buffer, size_t count) const override {
            ssize_t n = ::read(fd_, buffer, count);
            if (n == -1)
                throw std::system_error(errno, std::generic_category(), "Read failed.");
            return n;
        }


        // Write to FD
        size_t write(const char* texts, size_t count) const override {
            ssize_t n = ::write(fd_, texts, count);
            if (n == -1)
                throw std::system_error(errno, std::generic_category(), "Write failed.");
            return n;
        }

        #pragma endregion
    };
}

#endif