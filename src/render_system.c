#include "render_system.h"
#include <SDL2/Sdl_image.h>
#include <SDL2/Sdl_ttf.h>
const SDL_Color BLACK= {0,0,0,0};
bool load_texture_from_image(SDL_Renderer* renderer,char* path,SDL_Texture* texture){
    SDL_Surface* image_surface = IMG_Load(path);
    texture =  SDL_CreateTextureFromSurface(renderer,image_surface);
    if(image_surface == NULL){
        LOG("texture image not found");
        return false;
    }
    return true;
}
bool load_texture_from_font(SDL_Renderer* renderer,char* path,SDL_Texture* texture){
    TTF_Font* font = TTF_OpenFont(path,DEFAULT_FONT_SIZE);
    SDL_Surface* text_surface =  TTF_RenderText_Solid(font,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",BLACK);
    if(text_surface == NULL){
        LOG("font not found");
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer,text_surface);
    SDL_FreeSurface(text_surface);
    return true;
};
struct RenderSystem render_system_create(SDL_Window* window){
    struct RenderSystem render_system;
    render_system.renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    render_system.textureCounter = 0;
    return render_system;
}
bool render_system_add_texture(struct RenderSystem* render_system,SDL_Texture* texture){
    if(render_system->textureCounter < MAX_TEXTURES){
        render_system->textures[render_system->textureCounter++] = texture;
        return true;
    }
    LOG("render system full of textures");
    return false;
}
void render_system_destroy(struct RenderSystem* render_system){
    SDL_DestroyRenderer(render_system->renderer);
}
