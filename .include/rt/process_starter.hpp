#ifndef RT_PROCESS_STARTER_HPP
#define RT_PROCESS_STARTER_HPP

#include "../types/object.hpp"
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <unistd.h>
#include <sys/wait.h>
#endif

namespace asl::rt {
    
    // Spawn a new process
    class process_starter final : private types::object {
    private:
        // TODO: Incomplete process executor.
    };
}

#endif