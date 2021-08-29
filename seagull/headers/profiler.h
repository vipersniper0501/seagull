#ifndef PROFILER_HEADER_FILE
#define PROFILER_HEADER_FILE

#include "optick.h"

// I use the Optick Profiler for debugging long frames
#ifndef __MINGW32__
    #ifndef NDEBUG 
        #define SGL_PROFILE_MAIN() OPTICK_FRAME("MainThread");
        #define SGL_PROFILE_FUNCTION() OPTICK_EVENT();
        #define SGL_PROF_THEAD(NAME) OPTICK_THREAD(NAME);
    #else
        #define SGL_PROFILE_MAIN();
        #define SGL_PROFILE_FUNCTION();
        #define SGL_PROF_THEAD(NAME);
    #endif
#else // Don't use these if using mingw
    #define SGL_PROFILE_MAIN();
    #define SGL_PROFILE_FUNCTION();
    #define SGL_PROF_THEAD(NAME);
#endif

#endif