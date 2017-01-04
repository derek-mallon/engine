#ifndef IO_H
#define IO_H
#include <SDL2/SDL.h>
#include "file_handling.h"
ERR_error IO_init();
ERR_error load_texture_from_image(FIL_path path,SDL_Surface* surface);
#endif
