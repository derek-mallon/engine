#ifndef PHYSICS_H
#define PHYSICS_H

/*
#include "system.h"
#include <array.h>
#include <geometry.h>

#define PHYS_DEBUG 0x01

#define PHYS_BODY_STATIC 0x01
#define PHYS_BODY_REAL 0x02



typedef struct phys_body{
    rect rect;
    vec2 acceleration;
    vec2 velocity;
    vec2 friction;
    uint8_t flags;
}phys_body;

SYSTEM_L_DEC(phys_body)

typedef struct physics_collision{
    phys_body_handle body1,body2;
}physics_collision;

ARRAY_DEC(physics_collision)

void init_physics(uint8_t flags);
void destroy_physics();
phys_body create_phys_body(float x,float y,float width,float height,uint8_t flags,size_t layer_index);
void update_physics();

ARRAY(physics_collision) get_collisions();
*/

#endif
