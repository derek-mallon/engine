#include "sdl_wrapper.h"
#include <array.h>
#include <objectpool.h>
#include <stdint.h>
static color red = {1,0,0,1};
static color blue = {0,0,1,1};
static color green = {0,1,0,1};

const uint8_t STATIC = 0x01;
const uint8_t REAL = 0x02;

typedef struct phys_body{
    rect rect;
    vec2 acceleration;
    uint8_t flags;
}phys_body;

POOL_DEC(phys_body)
POOL_DEF(phys_body)

typedef struct layer{
    POOL(phys_body)  bodies;
}layer;

typedef struct phys_body_handle{
    size_t layer;
    size_t index;
}phys_body_handle;

bool aabb(rect rect1,rect rect2){
    return (fabsf(rect1.center.x-rect2.center.x) < (rect1.half_dim.x + rect2.half_dim.y) && fabsf(rect1.center.y-rect2.center.y) < (rect1.half_dim.y + rect2.half_dim.y));
}

typedef struct physics_collision{
    phys_body_handle body1,body2;
}physics_collision;

ARRAY_DEC(physics_collision)
ARRAY_DEF(physics_collision)
ARRAY_DEC(layer)
ARRAY_DEF(layer)

typedef struct physics_update_output{
    ARRAY(physics_collision)  *current_collisions;
}physics_update_output;

typedef struct physics_system{
    ARRAY(layer) layers;
    ARRAY(physics_collision) current_collisions;
}physics_system;

static physics_system data;

void render_rect(rect rect,color color){
    render_rect_outline(rect.center,scaler_multi(rect.half_dim,2),color);
    render_circle(rect.center,1,color);
}

void init_physics(){
    data.layers = ARRAY_CREATE(layer,1);
    data.layers.array[0].bodies = POOL_CREATE(phys_body,10);
    data.current_collisions = ARRAY_CREATE(physics_collision,10);
}

void clean_physics(){
    int i;
    for(i=0;i<data.layers.size;i++){
        POOL_DESTROY(phys_body,&data.layers.array[i].bodies);
    }
    ARRAY_DESTROY(layer,&data.layers);
    ARRAY_DESTROY(physics_collision,&data.current_collisions);
}

phys_body_handle add_phys_body(float x,float y,float width,float height,uint8_t flags,size_t layer_index){
    phys_body_handle handle;
    //Determine the layer
    if(layer_index >= data.layers.size){
        layer added;
        handle.layer = data.layers.size;
        added.bodies = POOL_CREATE(phys_body,10);
        ARRAY_ADD(layer,&data.layers,added);
    }else{
        handle.layer = layer_index;
    }
    
    vec2 center = {x,y};
    vec2 half_dim = {width/2,height/2};
    rect rect = {center,half_dim};
    vec2 acceleration = {0.0,0.0};
    phys_body body = {rect,acceleration,flags};

    phys_body* body_ptr = NULL;
    size_t index_of_next_free = 0;
    if((body_ptr = POOL_NEXT_FREE(phys_body,&data.layers.array[handle.layer].bodies,&index_of_next_free))){
        *body_ptr  = body;
        handle.index = index_of_next_free;
    }else{
        POOL_ADD(phys_body,&data.layers.array[handle.layer].bodies,body);
        handle.index = data.layers.array[handle.layer].bodies.array.size;
    }
        
    return handle;
}

uint8_t free_phys_body(phys_body_handle handle){
    if(handle.layer >= data.layers.size || data.layers.array[handle.layer].bodies.array.array[handle.index].alive == 0){
        return 0;
    }
    POOL_FREE(phys_body,&data.layers.array[handle.layer].bodies,handle.index);
    return 1;
}

void update_physics(){
   int i,iter1=0; 
   for(i=0;i<data.layers.size;i++){
       phys_body* next = NULL;
       while((next = POOL_NEXT(phys_body,&data.layers.array[i].bodies,iter1))){

       }
   }
}

/*
int main(){
    init_sdl("test",1600,1000,NULL,0);
    sdl_layer_output output;
    rect rect1 = {create_vec2(0,0),create_vec2(0.5,0.5)};
    rect rect2 = {create_vec2(-4,0),create_vec2(0.5,0.5)};
    vec2 pos1 = create_vec2(1.6,1.5);
    vec2 pos2 = create_vec2(-4.1,1.2);
    float x = -4;
    while(get_running()){
        x += 0.1;
        rect2.center.x = x;
        output = input_loop();
        render_rect(rect1,red);
        if(aabb(rect1,rect2)){
            render_rect(rect2,green);
        }else{
            render_rect(rect2,blue);
        }
        render_clear();
        update_physics();
    }
}
*/
