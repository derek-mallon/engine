#ifndef IO_H
#define IO_H
#include <SDL2/SDL.h>
#include "file_handling.h"
#include "mem.h"
ERR_error IO_init();
ERR_error IO_load_texture_from_image(FIL_path path,SDL_Surface* surface);

#endif
