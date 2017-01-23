#ifndef ASSET_H
#define ASSET_H
#include "mem.h"
#include "error.h"
#include "sdl_wrapper.h"
#include "all.h"
typedef struct AST_data AST_data;

struct AST_data{
    MEM_heap* texture_paths;
    MEM_heap* component_lib_paths;
    MEM_heap* audio_paths;
    WPR_sdl_data* sdl_wrapper_data;
};

void AST_lib_open(LIB_HANDLE* handle,UTI_str str);

void AST_lib_close(LIB_HANDLE handle);
void AST_init(MEM_heap_manager* manager);
#endif