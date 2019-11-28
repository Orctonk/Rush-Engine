#define RUSH_LINUX 1
#define RUSH_BUILD_SHARED 1

#if RUSH_WINDOWS
    #define PLATFORM "windows"
#elif RUSH_APPLE
    #define PLATFORM "apple"
#elif RUSH_LINUX
    #define PLATFORM "linux"
    #if RUSH_BUILD_SHARED
        #define RUSH_API __attribute__((visibility("default")))
    #else
        #define RUSH_API
    #endif
#endif

RUSH_API void print();