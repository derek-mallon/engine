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

enum MEMORY_LOC {
    FOREACH_MEM_LOC(GENERATE_ENUM)
};

static const char *MEMORY_LOC_STR[] = {
    FOREACH_MEM_LOC(GENERATE_STRING)
};

typedef void* LIB_HANDLE;
typedef void* LIB_FUNC;

typedef struct ALL_info ALL_info;
struct ALL_info{
    UTI_buff_stor project_name;
    UTI_buff_stor base_path;
    UTI_buff_stor texture_dir;
    UTI_buff_stor component_dir;
    UTI_buff_stor audio_dir;
    UTI_buff_stor bin_dir;
    UTI_buff_stor mem_binary;
    UTI_buff_stor self;
    size_t number_of_textures;
    size_t number_of_components;
    size_t number_of_audio_files;
};

#endif
