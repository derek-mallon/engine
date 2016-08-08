#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL2/Sdl.h>
#include <stdint.h>
struct SdlSystem{
    SDL_Window* window;
};
struct SdlSystem sdl_system_create(uint16_t screen_width,uint16_t screen_height,char* title);
void sdl_system_destroy(struct SdlSystem* sdl_system);
#endif
