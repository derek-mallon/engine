#ifndef RENDER_H
#define RENDER_H
/*
#include <array.h>
#include "sdl_wrapper.h"
#include "mem.h"
//Frame
typedef struct REN_frame{
    uint16_t x,y,width,height;
}frame;



typedef struct REN_animation{
    MEM_heap frames;
    size_t current_frame;
    size_t texture_index;
}animation;


typedef struct REN_sprite{
    size_t texture_index;
    double angle;
    MEM_heap animations;
    size_t current_animation;
    vec2 rotation_point;
    WPR_flip flip;
}sprite;

//Create an animation from frames in a straight line from the given cords
REN_animation REN_create_animation_from_strip(size_t texture_index,uint16_t number_of_frames,uint16_t sprite_width,uint16_t sprite_height,uint16_t x_offset,uint16_t y_offset);

void REN_destroy_animation(REN_animation* animation);
*/
#endif
