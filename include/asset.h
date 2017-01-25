#ifndef ASSET_H
#define ASSET_H
#include "mem.h"
#include "error.h"
#include <SDL2/Sdl.h>
#include "all.h"

typedef struct AST_data AST_data;
typedef struct AST_audio_data AST_audio_data;
typedef struct AST_texture_data AST_texture_data;

typedef uint8_t* AST_audio_buff;

struct AST_audio_data{
    UTI_buff_stor path;
    size_t wav_length;
    AST_audio_buff wav_buff;
    SDL_AudioSpec wav_spec;
};

struct AST_texture_data{
    UTI_buff_stor path;
    SDL_Texture* texture_buff;
};

struct AST_data{
    MEM_heap* texture_data;
    MEM_heap* audio_data;
};

LIB_HANDLE AST_lib_open(UTI_str str);
LIB_FUNC AST_get_func(LIB_HANDLE handle,UTI_str str);

void AST_lib_close(LIB_HANDLE handle);
void AST_init(MEM_heap_manager* manager);
#endif
