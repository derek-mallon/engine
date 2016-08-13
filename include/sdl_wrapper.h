#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL2/Sdl.h>
#include <stdint.h>
typedef struct SS{
    SDL_Window* window;
}SS;
SS SS_create(uint16_t screen_width,uint16_t screen_height,char* title);
void SS_destroy(SS* sdl_system);
#endif
