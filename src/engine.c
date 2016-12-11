#include "engine.h"
#include "physics.h"
#include "render.h"
#include <array.h>
#include <objectpool.h>





typedef struct entity_handle{
    size_t index;
}entity_handle;

typedef struct entity{
    sprite* sprite;
    phys_body* phys_body;
    entity_handle handle;
}entity;

POOL_DEC(entity)
POOL_DEF(entity)

typedef struct entity_system{
    POOL(entity) entities;
}entity_system;

static entity_system data;

void init_entity_system(){
    data.entities = POOL_CREATE(entity,10);
}

entity* create_entity(){
    entity output;
    entity* output_ptr;
    output.sprite = NULL;
    output.phys_body = NULL;
    size_t index = 0;
    if((output_ptr = POOL_NEXT_FREE(entity,&data.entities,&index))){
        output.handle.index = index;
        *output_ptr = output;
    }else{
        output.handle.index = data.entities.array.size;
        POOL_ADD(entity,&data.entities,output);
        output_ptr = POOL_GET(entity,&data.entities,output.handle.index);
    }
    return output_ptr;
}

uint8_t free_entity(entity* entity){
    return 0;
}


