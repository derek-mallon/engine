#ifndef GAME_OBJECT
#define GAME_OBJECT
#include "render_system.h"
#include <SDL2/Sdl.h>
#include <stdint.h>
struct XY{
    uint32_t x;
    uint32_t y;
};
struct Sprite{
    SDL_Rect* frames;
    size_t currentFrame;
    size_t texture;
};
struct GameObject{
    struct Sprite sprite;
    struct XY postion;
};
struct GameObjectManager{
    struct GameObject* gameObjects;
};
#endif
