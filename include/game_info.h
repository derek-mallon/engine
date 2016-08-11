#ifndef GAME_INFO_H
#define GAME_INFO_H
#include <stdint.h>
#include <SDL2/Sdl.h>
struct GameInfo{
    uint16_t screenWidth,screenHeight;
    float unitX,offSetX,unitY,offSetY,scaleX,scaleY;
    uint32_t timeElapsed;
    uint32_t counterPrev,timeSinceLastUpdate,deltaTime;
};
struct GameInfo game_info_create(SDL_Window* window);
void game_info_update(struct GameInfo* gameInfo);
#endif
