#ifndef RENDER_H
#define RENDER_H
#include <array.h>
#include "sdl_wrapper.h"
#include "system.h"
//Frame
typedef struct frame{
    uint16_t x,y,width,height;
}frame;

ARRAY_DEC(frame)


typedef struct animation{
    ARRAY(frame) frames;
    size_t current_frame;
    size_t texture_index;
}animation;


ARRAY_DEC(animation)
typedef struct sprite{
    size_t texture_index;
    double angle;
    ARRAY(animation) animations;
    size_t current_animation;
    vec2 rotation_point;
    flip flip;
}sprite;

SYSTEM_DEC(sprite)
//Create an animation from frames in a straight line from the given cords
animation create_animation_from_strip(size_t texture_index,uint16_t number_of_frames,uint16_t sprite_width,uint16_t sprite_height,uint16_t x_offset,uint16_t y_offset);

void destroy_animation(animation* animation);
#endif
