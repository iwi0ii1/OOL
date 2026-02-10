#ifndef RUNTIME_PROCESS_STATE_HPP
#define RUNTIME_PROCESS_STATE_HPP

#include <string_view>

#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <unistd.h>
#endif

inline namespace ool {
    namespace runtime {

        // The current state of a process
        class process_state final {
            // TODO: Incomplete process_state
        };
    }
}

#endif