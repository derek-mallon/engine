#ifndef GAME_INFO_H
#define GAME_INFO_H
#include <stdint.h>
struct GameInfo{
    uint16_t screenWidth,screenHeight;
    float unitX,unitY,scaleX,scaleY;
    uint32_t timeElapsed;
    uint64_t counterPrev,deltaTime,timeSinceLastUpdate;
};
#endif
