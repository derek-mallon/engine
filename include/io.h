#ifndef IO_H
#define IO_H
#include <SDL2/SDL.h>
#include "file_handling.h"
#include "mem.h"
typedef enum IO_status IO_status;
enum IO_status{
    IO_CLOSED,
    IO_READY,
};
void IO_init();
void IO_close();
ERR_error IO_load_texture_from_image(FIL_path path,SDL_Surface* surface);
ERR_error IO_load_heap_binary(FIL_path*,MEM_heap* heap);
ERR_error IO_save_heap_binary(FIL_path*,MEM_heap* heap);
ERR_error IO_load_manager_binary(FIL_path*,MEM_heap_manager* manager);
ERR_error IO_save_manager_binary(FIL_path*,MEM_heap_manager* manager);
IO_status IO_get_status();

#endif
