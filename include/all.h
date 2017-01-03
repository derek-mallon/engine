#ifndef ALL_H
#define ALL_H
    #if defined(__linux__) || (__APPLE__)
        #define UNIX
    #endif
    #if defined(_WIN32)
        #define WIN
    #endif
#endif
