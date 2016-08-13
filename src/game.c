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
#include "game_info.h"
#include "game_object.h"
//Adds text to the beginning of a file, the file doesn't exist create it.
int main(){
    SS sdl_system =  SS_create(1600,1000,"engine");
    RS render_system = RS_create(sdl_system.window);
    GI game_info = GI_create(sdl_system.window);
    SDL_Event event;
    bool running = true;
    GO_Sprite sprite = SPRITE_CREATE(1, //Texture
            0,0,5,5, //Frame 1
            5,0,10,10); //Frame 2
    printf("%d, %d",sprite.frames[0].x,sprite.frames[1].x);
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
        int x,y;
        SDL_GetMouseState(&x,&y);
        char buf[100];
        sprintf(buf,"%f,%f",x/game_info.unitX + game_info.offSetX,y/game_info.unitY + game_info.offSetY);
        SDL_SetWindowTitle(sdl_system.window,buf);
        game_info_update(&game_info);
        //render_system_update(&render_system);
    }
    render_system_destroy(&render_system);
    sdl_system_destroy(&sdl_system);
    return 0;
}
