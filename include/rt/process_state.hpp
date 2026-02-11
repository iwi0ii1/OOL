#ifndef RT_PROCESS_STATE_HPP
#define RT_PROCESS_STATE_HPP

#include "../types/object.hpp"
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <unistd.h>
#endif

namespace ool::rt {

    // The current state of a process
    class process_state final : private types::object {
    public:
        // TODO: Incomplete process_state
    };
}

#endif