#include "game_info.h"
const float WIDTH_IN_UNITS = 16;
const float HEIGHT_IN_UNITS = 10;
const float IDEAL_UNIT_X = 100;
const float IDEAL_UNIT_Y = 100;
struct GameInfo game_info_create(SDL_Window* window){
    int screen_width, screen_height;
    SDL_GetWindowSize(window,&screen_width,&screen_height);
    float unit_x = screen_width/WIDTH_IN_UNITS;
    float unit_y = -screen_height/HEIGHT_IN_UNITS;
    struct GameInfo game_info = {screen_width,screen_height,unit_x,-WIDTH_IN_UNITS/2,unit_y,HEIGHT_IN_UNITS/2,IDEAL_UNIT_X/unit_x,IDEAL_UNIT_Y/unit_y,SDL_GetTicks(),SDL_GetTicks(),0,0};
    return game_info;
}
void game_info_update(struct GameInfo* game_info){
    uint16_t counter_current = SDL_GetTicks();
    game_info->deltaTime = (counter_current - game_info->counterPrev);
    if(game_info->deltaTime == 0){
        game_info->deltaTime++;
    }
    game_info->counterPrev = counter_current;
    game_info->timeElapsed = SDL_GetTicks();
}
