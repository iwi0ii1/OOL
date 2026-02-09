#ifndef RUNTIME_PROCESS_EXECUTOR_HPP
#define RUNTIME_PROCESS_EXECUTOR_HPP

#include <string_view>

#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <unistd.h>
#include <sys/wait.h>
#endif

inline namespace ool {
    namespace runtime {
    
        // Spawn a new process
        class process_executor final {
        private:
            // TODO: Incomplete process executor.
        };
    }
}

#endif