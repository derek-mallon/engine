#ifndef ALL_H
#define ALL_H
    #if defined(__linux__) || (__APPLE__)
        #define UNIX
    #endif
    #if defined(_WIN32)
        #define WIN
    #endif

#define FOREACH_MEM_LOC(MEM_LOC) \
        MEM_LOC(MEM_LOC_INFO)\
        MEM_LOC(MEM_LOC_WPR_SDL_DATA)\
        MEM_LOC(MEM_LOC_AST_DATA)\
        MEM_LOC(MEM_LOC_AUDIO_DATA)\
        MEM_LOC(MEM_LOC_TEXTURE_DATA)\
        MEM_LOC(MEM_LOC_ENTITIES)\
        MEM_LOC(MEM_LOC_TOTAL)


#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

#define UTI_DEFAULT_NAME_SIZE 50
#include <stdlib.h>



enum MEMORY_LOC {
    FOREACH_MEM_LOC(GENERATE_ENUM)
};

static const char *MEMORY_LOC_STR[] = {
    FOREACH_MEM_LOC(GENERATE_STRING)
};

typedef void* LIB_HANDLE;
typedef void* LIB_FUNC;


#endif
