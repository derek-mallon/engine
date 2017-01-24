#ifndef ASSET_H
#define ASSET_H
#include "mem.h"
#include "error.h"
#include "sdl_wrapper.h"
#include "all.h"

typedef void* AST_FUNC;
typedef struct AST_data AST_data;

struct AST_data{
    MEM_heap* texture_paths;
    MEM_heap* component_lib_paths;
    MEM_heap* audio_paths;
    WPR_sdl_data* sdl_wrapper_data;
};

LIB_HANDLE AST_lib_open(UTI_str str);
AST_FUNC AST_get_func(LIB_HANDLE handle,UTI_str str);

void AST_lib_close(LIB_HANDLE handle);
void AST_init(MEM_heap_manager* manager);
#endif
