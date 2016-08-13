#include "sdl_wrapper.h"
#include "util.h"
#include <SDL2/Sdl_image.h>
#include <SDL2/Sdl_ttf.h>
SS SS_create(uint16_t screen_width,uint16_t screen_height,char* title){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
        LOG("could not init SDL2");
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
         LOG("could not init SDL2_image");
    }
    if(TTF_Init() == -1){
        LOG("could not init SDL2_ttf");
    }
    SS sdl_system;
    sdl_system.window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screen_width,screen_height,SDL_WINDOW_SHOWN);
    return sdl_system;
}
void SS_destroy(SS* sdl_system){
    SDL_DestroyWindow(sdl_system->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
