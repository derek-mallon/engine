#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H
#include <SDL2/Sdl.h>
#include "util.h"
bool load_texture_from_image(SDL_Renderer* renderer,char* path,SDL_Texture* texture);
bool load_texture_from_font(SDL_Renderer* renderer,char* path,SDL_Texture* texture);
struct RenderSystem{
    SDL_Texture* textures[MAX_TEXTURES];
    SDL_Renderer* renderer;
    size_t textureCounter;
};
struct RenderSystem render_system_create(SDL_Window* window);
void render_system_update(struct RenderSystem* render_system);
bool render_system_add_texture(struct RenderSystem* render_system,SDL_Texture* texture);
void render_system_destroy(struct RenderSystem* render_system);
#endif
