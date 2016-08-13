#include "render_system.h"
#include <SDL2/Sdl_image.h>
#include <SDL2/Sdl_ttf.h>
RS RS_create(SDL_Window* window){
    RS render_system;
    render_system.renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    render_system.textureCounter = 0;
    return render_system;
}
bool RS_add_texture(RS* render_system,SDL_Texture* texture){
    if(render_system->textureCounter < MAX_TEXTURES){
        render_system->textures[render_system->textureCounter++] = texture;
        return true;
    }
    LOG("render system full of textures");
    return false;
}
void RS_destroy(RS* render_system){
    SDL_DestroyRenderer(render_system->renderer);
}
void RS_render(RS* render_system,RenderInfo* render_info){

}
