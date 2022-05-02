#include <Rushpch.h>

#ifdef RUSH_PLATFORM_LINUX
    #include "FileLinux.inl"
#elif defined(RUSH_PLATFORM_WINDOWS)
    #include "FileWindows.inl"
#else
    #error "Platform not supported"
#endif