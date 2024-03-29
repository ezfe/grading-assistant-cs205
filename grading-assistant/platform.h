#ifndef PLATFORM_H
#define PLATFORM_H

#define GA_PLATFORM_APPLE "APPLE"
#define GA_PLATFORM_WINDOWS "WINDOWS"
#define GA_PLATFORM_LINUX "LINUX"
#define GA_PLATFORM_UNKNOWN "UNKNOWN"

#ifdef __APPLE__
    #define GA_PLATFORM GA_PLATFORM_APPLE
    #define GA_APPLE_ACTIVE true
#elif _WIN32
    #define GA_PLATFORM GA_PLATFORM_WINDOWS
    #define GA_WINDOWS_ACTIVE true
#elif __linux__
    #define GA_PLATFORM GA_PLATFORM_LINUX
    #define GA_LINUX_ACTIVE true
#else
    #define GA_PLATFORM GA_PLATFORM_UNKNOWN
    #define GA_UNKNOWN_ACTIVE true
#endif

#endif // PLATFORM_H
