#include <Rushpch.h>

#ifdef RUSH_PLATFORM_LINUX
    #include "FileLinux.cpp"
#elif defined(RUSH_PLATFORM_WINDOWS)
    #include "FileWindows.cpp"
#else
    #error "Platform not supported"
#endif