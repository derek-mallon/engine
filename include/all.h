#ifndef ALL_H
#define ALL_H
    #if defined(__linux__) || (__APPLE__)
        #define UNIX
    #endif
    #if defined(_WIN32)
        #define WIN
    #endif
#define FOREACH_MEM_LOC(MEM_LOC) \
        MEM_LOC(MEM_LOC_COM_DATA)\
        MEM_LOC(MEM_LOC_COM_LIB_HANDLES)\
        MEM_LOC(MEM_LOC_WPR_SDL_DATA)\
        MEM_LOC(MEM_LOC_AST_DATA)\
        MEM_LOC(MEM_LOC_AUDIO_PATHS)\
        MEM_LOC(MEM_LOC_COMPONENT_LIB_PATHS)\
        MEM_LOC(MEM_LOC_TEXTURE_PATHS)\
        MEM_LOC(MEM_LOC_STR_BUFF)\
        MEM_LOC(MEM_LOC_TOTAL)

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

enum MEMORY_LOC {
    FOREACH_MEM_LOC(GENERATE_ENUM)
};

static const char *MEMORY_LOC_STR[] = {
    FOREACH_MEM_LOC(GENERATE_STRING)
};

typedef void* LIB_HANDLE;
#endif
