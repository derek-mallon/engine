#ifndef ENGINE_H
#define ENGINE_H
#include "sdl_wrapper.h"
#include <stdint.h>
typedef struct sprite{
    size_t texture_index;
    double angle;
    animation* animations;
    size_t number_of_animations;
    size_t current_animation;
    vec2 rotation_point;
    flip flip;
}sprite;

sprite create_sprite(animation* animations,size_t number_of_animations);

void destroy_sprite(sprite* sprite);
#endif
