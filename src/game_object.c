#include "game_object.h"
#include <stdarg.h>
OBJECT_POOL_DEF(GO_Sprite)
GO_Sprite GO_sprite_create(size_t texture,int numb_of_frames,...){
    va_list ap;
    int i;
    int counter = 0;
    GO_Sprite sprite = {malloc(sizeof(SDL_Rect)*numb_of_frames),{0,0},0.0,{0,0},texture,0,numb_of_frames};
    va_start(ap, (numb_of_frames*4));
    for(i=0;i<(numb_of_frames*4);i++){
        int t = i % 4;
        if(t == 0){
            sprite.frames[counter].x = va_arg(ap,int);
        }
        if(t == 1){
            sprite.frames[counter].y = va_arg(ap,int);
        }
        if(t == 2){
            sprite.frames[counter].w = va_arg(ap,int);
        }
        if(t == 3){
            sprite.frames[counter].h = va_arg(ap,int);
            counter++;
        }
    }
    va_end(ap);
    return sprite;
}
GO_Sprite GO_sprite_create_macro(int macro_arg_count,size_t texture,...){
    printf("marco arg_count %d\n",macro_arg_count);
    int numb_of_frames = macro_arg_count -1;
    va_list ap;
    int i;
    int counter = 0;
    GO_Sprite sprite = {malloc(sizeof(SDL_Rect)*numb_of_frames),{0,0},0.0,{0,0},texture,0,numb_of_frames};
    va_start(ap, (numb_of_frames*4));
    for(i=0;i<(numb_of_frames*4);i++){
        int t = i % 4;
        if(t == 0){
            sprite.frames[counter].x = va_arg(ap,int);
        }
        if(t == 1){
            sprite.frames[counter].y = va_arg(ap,int);
        }
        if(t == 2){
            sprite.frames[counter].w = va_arg(ap,int);
        }
        if(t == 3){
            sprite.frames[counter].h = va_arg(ap,int);
            counter++;
        }
    }
    va_end(ap);
    return sprite;
}
void GO_sprite_destroy(GO_Sprite* sprite){
    free(sprite->frames);
}
RenderInfo GO_render(GO_Sprite* sprite){
    RenderInfo render_info = {sprite->frames[sprite->currentFrame],sprite->pos,sprite->angle,sprite->center,sprite->texture};
    if(sprite->currentFrame < sprite->numbOfFrames-1){
        sprite->currentFrame++;
    }else{
        sprite->currentFrame = 0;
    }
    return render_info;
}
