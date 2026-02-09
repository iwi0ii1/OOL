#ifndef PROC_ENV_VAR_HPP
#define PROC_ENV_VAR_HPP

#include <cstdlib>

#ifdef __unix__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

inline namespace ool {
    // Where u manipulate environment variables
    namespace env_var {
        // TODO: Implement functions that manipulates `extern char** environ`
    }
}

#endif