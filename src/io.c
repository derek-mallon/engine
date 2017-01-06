#include "io.h"
#include "error.h"
#include <SDL2/SDL_image.h>

void IO_init(){
    ERR_ASSERT(WPR_STATUS == WPR_READY,"SDL wrapper not inited")
    ERR_ASSERT(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG,"Couldn't init SDL_image: %s",SDL_GetError());
    IO_STATUS = IO_READY;
}


ERR_error IO_load_texture_from_image(FIL_path path,SDL_Surface* surface){
    surface= IMG_Load(path.raw);
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't load the image at %s: %s",path.raw,SDL_GetError());
        return ERR_BAD;
    }
    return ERR_GOOD;
}

void IO_close(){
    IMG_Quit();
    IO_STATUS = IO_CLOSED;
}


