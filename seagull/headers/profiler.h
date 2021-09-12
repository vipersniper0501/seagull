#ifndef PROFILER_HEADER_FILE
#define PROFILER_HEADER_FILE


// I use the Optick Profiler for debugging long frames
#ifndef __MINGW32__
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
        #include "optick.h"
        #ifndef NDEBUG // yes debugging
            #define SGL_PROFILE_MAIN() OPTICK_FRAME("MainThread")
            #define SGL_PROFILE_FUNCTION() OPTICK_EVENT()
            #define SGL_PROF_THEAD(NAME) OPTICK_THREAD(NAME)
        #else
            #define SGL_PROFILE_MAIN() do {} while(0);
            #define SGL_PROFILE_FUNCTION() do {} while(0);
            #define SGL_PROF_THEAD(NAME) do {} while(0);
        #endif
    #else
        #define SGL_PROFILE_MAIN() do {} while(0);
        #define SGL_PROFILE_FUNCTION() do {} while(0);
        #define SGL_PROF_THEAD(NAME) do {} while(0);
    #endif
#else
    #define SGL_PROFILE_MAIN() do {} while(0);
    #define SGL_PROFILE_FUNCTION() do {} while(0);
    #define SGL_PROF_THEAD(NAME) do {} while(0);
#endif


#endif
