#ifndef GAME_OBJECT
#define GAME_OBJECT
#include "render_system.h"
#include "util.h"
#include <SDL2/Sdl.h>
#include <stdint.h>
#include <objectpool.h>
#define SPRITE_CREATE(...) GO_sprite_create_macro(VA_NUM_ARGS(__VA_ARGS__),__VA_ARGS__)
typedef struct GO_Sprite{
    SDL_Rect* frames;
    SDL_Point pos;
    double angle;
    SDL_Point center;
    size_t texture;
    size_t currentFrame;
    int numbOfFrames;
}GO_Sprite;
GO_Sprite GO_sprite_create(size_t texture,int numb_of_frames,...);
GO_Sprite GO_sprite_create_macro(int macro_arg_count,size_t texture,...);
void GO_sprite_destroy(GO_Sprite* sprite);
RenderInfo GO_sprite_render(GO_Sprite* sprite);
OBJECT_POOL_DEC(GO_Sprite)
#endif
