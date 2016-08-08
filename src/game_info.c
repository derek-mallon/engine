#include "game_info.h"
#include <SDL2/Sdl.h>
const float width_in_units = 16;
const float height_in_units = 10;
const float ideal_unit_x = 100;
const float ideal_unit_y = 100;
struct GameInfo game_info_create(SDL_Window* window){
    int screen_width, screen_height;
    SDL_GetWindowSize(window,&screen_width,&screen_height);
    float unit_x = screen_width/width_in_units;
    float unit_y = screen_height/height_in_units;
    struct GameInfo game_info = {screen_width,screen_height,unit_x,unit_y,ideal_unit_x/unit_x,ideal_unit_y/unit_y,SDL_GetTicks(),SDL_GetPerformanceCounter(),0,0};
    return game_info;
}
void game_info_update(struct GameInfo* gameInfo){
    uint16_t counter_current = SDL_GetPerformanceCounter();
}
