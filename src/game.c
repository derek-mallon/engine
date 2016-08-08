#include <SDL2/Sdl.h>
#include <SDL2/Sdl_image.h>
#include <SDL2/Sdl_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h> 
#include <hashtable.h>
#include "util.h"
#include "sdl_wrapper.h"
#include "render_system.h"
//Adds text to the beginning of a file, the file doesn't exist create it.
int main(){
    struct SdlSystem sdl_system =  sdl_system_create(1600,1000,"engine");
    struct RenderSystem render_system = render_system_create(sdl_system.window);
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
    render_system_destroy(&render_system);
    sdl_system_destroy(&sdl_system);
    return 0;
}
