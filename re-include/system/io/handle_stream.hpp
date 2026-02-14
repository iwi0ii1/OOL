/*
Windows only.
HANDLE wrapper.
*/

#pragma once

#ifdef _WIN32

#include "../../base/stream.hpp"

#include <windows.h>

namespace asl::system::io {
    // Handle wrapper
    // @note Doesn't create file if doesn't exist.
    class handle_stream final : public base::stream {
        HANDLE handle;
    public:
        // TODO: Windows: handle stream
        handle_stream() {
            
        }
    };
}

#endif