#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <SDL2/Sdl.h>
#include "util.h"
typedef struct RS{
    SDL_Texture* textures[MAX_TEXTURES];
    SDL_Renderer* renderer;
    size_t textureCounter;
}RS;
RS RS_create(SDL_Window* window);
bool RS_add_texture(RS* render_system,SDL_Texture* texture);
void RS_render(RS* render_system,RenderInfo* renderInfo);
void RS_destroy(RS* render_system);
#endif
