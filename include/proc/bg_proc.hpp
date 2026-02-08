#ifndef PROC_BG_PROC_HPP
#define PROC_BG_PROC_HPP
#endif

#ifdef _WIN32

#elif __unix__

#endif


namespace alt {
    namespace proc {

        #ifdef __unix__
        // A wrapper around a daemon
        class daemon final {
            // TODO: Fill it up
        };

        #elif _WIN32
        // A wrapper around a service
        class service final {
            // TODO: Fill it up
        };

        #endif
    }
}