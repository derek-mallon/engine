#include "sdl_wrapper.h"
#include "physics.h"
#include <objectpool.h>
#include <stdint.h>
static color red = {1,0,0,1};
static color blue = {0,0,1,1};
static color green = {0,1,0,1};




const float TIME_FACTOR = 0.01;


POOL_DEC(phys_body)
POOL_DEF(phys_body)

typedef struct layer{
    POOL(phys_body)  bodies;
}layer;


bool aabb(rect rect1,rect rect2){
    return (fabsf(rect1.center.x-rect2.center.x) < (rect1.half_dim.x + rect2.half_dim.x) && fabsf(rect1.center.y-rect2.center.y) < (rect1.half_dim.y + rect2.half_dim.y));
}


ARRAY_DEF(physics_collision)
ARRAY_DEC(layer)
ARRAY_DEF(layer)


typedef struct physics_system{
    ARRAY(layer) layers;
    ARRAY(physics_collision) current_collisions;
    uint8_t flags;
}physics_system;

static physics_system data;

void render_rect(rect rect,color color){
    render_rect_outline(rect.center,scaler_multi(rect.half_dim,2),color);
    render_circle(rect.center,1,color);
}

void init_physics(uint8_t flags){
    data.layers = ARRAY_CREATE(layer,1);
    data.layers.array[0].bodies = POOL_CREATE(phys_body,10);
    data.current_collisions = ARRAY_CREATE(physics_collision,10);
    data.flags = flags;
}


void clean_physics(){
    int i;
    for(i=0;i<data.layers.size;i++){
        POOL_DESTROY(phys_body,&data.layers.array[i].bodies);
    }
    ARRAY_DESTROY(layer,&data.layers);
    ARRAY_DESTROY(physics_collision,&data.current_collisions);
}

phys_body* get_phys_body(phys_body_handle handle){
    if(data.layers.array[handle.layer].bodies.array.array[handle.index].alive == 0){
        return NULL;
    }
    return &data.layers.array[handle.layer].bodies.array.array[handle.index].item;
}

phys_body* add_phys_body(float x,float y,float width,float height,uint8_t flags,size_t layer_index){
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
    vec2 velocity = {0.0,0.0};
    vec2 friction = {0.0,0.0};
    phys_body body = {rect,acceleration,velocity,friction,flags};
    phys_body* body_ptr = NULL;
    size_t index_of_next_free = 0;
    if((body_ptr = POOL_NEXT_FREE(phys_body,&data.layers.array[handle.layer].bodies,&index_of_next_free))){
        *body_ptr  = body;
        handle.index = index_of_next_free;
        body_ptr->handle = handle;
    }else{
        handle.index = POOL_ADD(phys_body,&data.layers.array[handle.layer].bodies,body);
        body_ptr = get_phys_body(handle);
        body_ptr->handle = handle;
    }
    return body_ptr;
}

uint8_t free_phys_body(phys_body* body){
    phys_body_handle handle = body->handle;
    if(data.layers.array[handle.layer].bodies.array.array[handle.index].alive == 0){
        return 0;
    }
    POOL_FREE(phys_body,&data.layers.array[handle.layer].bodies,handle.index);
    return 1;
}

//Always set an acceleration never add one
void update_phys_body(phys_body* body){
    body->acceleration = approach_clamp_vec2(body->acceleration,scaler_vec2(body->friction,((float)get_delta_time())*TIME_FACTOR),0.0,0.5);
    body->velocity = scaler_vec2(body->acceleration,((float)get_delta_time())*TIME_FACTOR);
    body->rect.center = add_vec2(body->rect.center,scaler_vec2(body->velocity,((float)get_delta_time())*TIME_FACTOR));
}

void update_physics(){
   int i; 

   //reset the current collisions
   data.current_collisions.size = 0;

   for(i=0;i<data.layers.size;i++){
       POOL_ITER(phys_body) iterator = POOL_ITER_CREATE(phys_body);
       //Determine the collisions an move the bodies
       while(POOL_NEXT(phys_body,&data.layers.array[i].bodies,&iterator)){
           //only move and check for collisions if not static
           if(!(iterator.item->flags & PHYS_BODY_STATIC)){
               vec2 prev_pos  = iterator.item->rect.center;
               update_phys_body(iterator.item);
               uint8_t collided = 0;
               POOL_ITER(phys_body) second_iterator = POOL_ITER_CREATE(phys_body);
               while(POOL_NEXT(phys_body,&data.layers.array[i].bodies,&second_iterator)){
                   if(iterator.index != second_iterator.index){
                       if(aabb(iterator.item->rect,second_iterator.item->rect)){
                           collided = 1;
                           phys_body_handle body1 = {i,iterator.index};
                           phys_body_handle body2 = {i,second_iterator.index};
                           if(iterator.item->acceleration.x < 0){
                               iterator.item->rect.center.x = second_iterator.item->rect.center.x + second_iterator.item->rect.half_dim.x + iterator.item->rect.half_dim.x;
                           }
                           if(iterator.item->acceleration.x > 0){
                               iterator.item->rect.center.x = second_iterator.item->rect.center.x - second_iterator.item->rect.half_dim.x - iterator.item->rect.half_dim.x;
                           }
                           if(iterator.item->acceleration.y < 0){
                               iterator.item->rect.center.y = second_iterator.item->rect.center.y + second_iterator.item->rect.half_dim.y + iterator.item->rect.half_dim.y;
                           }
                           if(iterator.item->acceleration.y > 0){
                               iterator.item->rect.center.y = second_iterator.item->rect.center.y - second_iterator.item->rect.half_dim.y - iterator.item->rect.half_dim.y;
                           }
                           physics_collision collision = {body1,body2};
                           ARRAY_ADD(physics_collision,&data.current_collisions,collision);
                       }
                   }
               }
               if(collided){
                   iterator.item->acceleration = create_vec2(0.0,0.0);
               }
               if(data.flags & PHYS_DEBUG)
                   render_rect(iterator.item->rect,red);
           }else{
               if(data.flags & PHYS_DEBUG)
                   render_rect(iterator.item->rect,green);
           }
       }
   }
}

ARRAY(physics_collision) get_collisions(){
    return data.current_collisions;
}

