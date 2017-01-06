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
IO_status IO_STATUS = IO_CLOSED;
void IO_init();
void IO_close();
ERR_error IO_load_texture_from_image(FIL_path path,SDL_Surface* surface);
ERR_error IO_load_mem_binary(FIL_path,MEM_heap_manager* manager);
ERR_error IO_save_mem_binary(FIL_path,MEM_heap_manager* manager);

#endif
