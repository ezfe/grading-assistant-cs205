#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef __APPLE__
    #define GA_PLATFORM "APPLE"
#elif _WIN32
    #define GA_PLATFORM "WINDOWS"
#elif __linux__
    #define GA_PLATFORM "LINUX"
#else
    #define GA_PLATFORM "UNKNOWN"
#endif

#endif // PLATFORM_H
