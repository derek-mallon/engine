#include "io.h"
#include "error.h"
#include <SDL2/SDL_image.h>

ERR_error IO_init(){
    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't init SDL_image: %s",SDL_GetError());
        return ERR_BAD;
    }
    return ERR_GOOD;
}


ERR_error load_texture_from_image(FIL_path path,SDL_Surface* surface){
    surface= IMG_Load(path.raw);
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't load the image at %s: %s",path.raw,SDL_GetError());
        return ERR_BAD;
    }
    return ERR_GOOD;
}
