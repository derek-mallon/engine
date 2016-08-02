#include <SDL2/Sdl.h>
#include <SDL2/Sdl_image.h>
#include <SDL2/Sdl_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h> 
#include <hashtable.h>
#include "util.h"
#define MAX_TEXTURES 10
typedef enum {false,true} bool;
const uint32_t UPDATE_TIME_STEP_MS;
const SDL_Color BLACK= {0,0,0,0};
const uint8_t DEFAULT_FONT_SIZE;
struct GameGlobalInfo{
    uint16_t screenWidth,screenHeight;
    float unitX,unitY,scaleX,scaleY;
    uint32_t timeElapsed,deltaTime,timeSinceLastUpdate;
};
struct Handle{
    size_t index;
    uint16_t counter;
};
//Adds text to the beginning of a file, the file doesn't exist create it.
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
struct SdlSystem{
    SDL_Window* window;
};
struct SdlSystem sdl_system_create(uint16_t screen_width,uint16_t screen_height,char* title){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        LOG("could not init SDL2");
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
         LOG("could not init SDL2_image");
    }
    if(TTF_Init() == -1){
        LOG("could not init SDL2_ttf");
    }
    struct SdlSystem sdl_system;
    sdl_system.window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screen_width,screen_height,SDL_WINDOW_SHOWN);
    return sdl_system;
}
void sdl_system_destroy(struct SdlSystem* sdl_system){
    SDL_DestroyWindow(sdl_system->window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
struct Sprite{
    SDL_Rect* rects;
};
struct RenderSystem{
    SDL_Texture* textures[MAX_TEXTURES];
    SDL_Renderer* renderer;
    size_t textureCounter;
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
int main(){
    char* test = read_file("log.txt");
    printf("%s",test);
    free(test);
    LOG("NEW GAME");
    Hashtable test_data = parse_data("test.data");
    struct SdlSystem sdl_system =  sdl_system_create(1600,1000,"engine");
    SDL_Event event;
    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                running = false;
            }
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
            }
        }
    }
    return 0;
}
