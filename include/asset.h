#ifndef ASSET_H
#define ASSET_H
#include "mem.h"
#include "error.h"
#include "sdl_wrapper.h"
#include "all.h"

typedef struct AST_data AST_data;
typedef struct AST_audio_data AST_audio_data;
typedef struct AST_texture_data AST_texture_data;

typedef enum AST_status AST_status; 
typedef uint8_t* AST_audio_buff;


enum AST_status{
    AST_CLOSED,
    AST_READY
};

struct AST_audio_data{
    size_t wav_length;
    AST_audio_buff wav_buff;
};

struct AST_texture_data{
    WPR_texture_ptr texture_buff;
};

struct AST_data{
    MEM_heap* texture_data;
    MEM_heap* audio_data;
    WPR_sdl_data* sdl_data;
};

LIB_HANDLE AST_lib_open(const char* str);

LIB_FUNC AST_get_func(LIB_HANDLE handle,const char* str);

void AST_lib_close(LIB_HANDLE handle);

ERR_error AST_init(MEM_heap_manager* manager);

void AST_load(MEM_heap_manager* manager);
void AST_shudown(MEM_heap_manager* manager);

AST_status AST_get_status();

ERR_error AST_load_texture(AST_data* data,AST_texture_data* texture,const char* path);

#endif
