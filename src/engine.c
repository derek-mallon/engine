#include "engine.h"
#include "physics.h"
#include "render.h"
#include "system.h"
#include <array.h>
#include <objectpool.h>

typedef struct entity{
    sprite* sprite;
    phys_body* phys_body;
}entity;

SYSTEM_DEC(entity)

SYSTEM_DEF(entity)

entity create_entity(){
    entity output = {NULL,NULL};
    return output;
}
