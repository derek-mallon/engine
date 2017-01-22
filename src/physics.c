#include "physics.h"
#include "sdl_wrapper.h"
/*
static color red = {1,0,0,1};
static color blue = {0,0,1,1};
static color green = {0,1,0,1};



SYSTEM_L_DEF(phys_body)

const float TIME_FACTOR = 0.01;





bool aabb(rect rect1,rect rect2){
    return (fabsf(rect1.center.x-rect2.center.x) < (rect1.half_dim.x + rect2.half_dim.x) && fabsf(rect1.center.y-rect2.center.y) < (rect1.half_dim.y + rect2.half_dim.y));
}


ARRAY_DEF(physics_collision)


typedef struct physics_system{
    ARRAY(physics_collision) current_collisions;
    uint8_t flags;
}physics_system;

static physics_system data;

void render_rect(rect rect,color color){
    render_rect_outline(rect.center,scaler_multi(rect.half_dim,2),color);
    render_circle(rect.center,1,color);
}

void init_physics(uint8_t flags){
    data.current_collisions = ARRAY_CREATE(physics_collision,10);
    data.flags = flags;
    phys_body_system_init(1,1);
}


void destroy_physics(){
    phys_body_system_destroy();
    ARRAY_DESTROY(physics_collision,&data.current_collisions);
}


phys_body create_phys_body(float x,float y,float width,float height,uint8_t flags,size_t layer_index){
    vec2 center = {x,y};
    vec2 half_dim = {width/2,height/2};
    rect rect = {center,half_dim};
    vec2 acceleration = {0.0,0.0};
    vec2 velocity = {0.0,0.0};
    vec2 friction = {0.0,0.0};
    phys_body body = {rect,acceleration,velocity,friction,flags};
    return body;
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

   for(i=0;i<system_.layers.size;i++){
       POOL_ITER(phys_body_component) iterator = POOL_ITER_CREATE(phys_body_component);
       //Determine the collisions an move the bodies
       while(POOL_NEXT(phys_body_component,&system_.layers.array[i].components,&iterator)){
           //only move and check for collisions if not static
           if(!(iterator.item->item.flags & PHYS_BODY_STATIC)){
               vec2 prev_pos  = iterator.item->item.rect.center;
               update_phys_body(&iterator.item->item);
               uint8_t collided = 0;
               POOL_ITER(phys_body_component) second_iterator = POOL_ITER_CREATE(phys_body_component);
               while(POOL_NEXT(phys_body_component,&system_.layers.array[i].components,&second_iterator)){
                   if(iterator.index != second_iterator.index){
                       if(aabb(iterator.item->item.rect,second_iterator.item->item.rect)){
                           collided = 1;
                           phys_body_handle body1 = iterator.item->handle;
                           phys_body_handle body2 = second_iterator.item->handle;
                           if(iterator.item->item.acceleration.x < 0){
                               iterator.item->item.rect.center.x = second_iterator.item->item.rect.center.x + second_iterator.item->item.rect.half_dim.x + iterator.item->item.rect.half_dim.x;
                           }
                           if(iterator.item->item.acceleration.x > 0){
                               iterator.item->item.rect.center.x = second_iterator.item->item.rect.center.x - second_iterator.item->item.rect.half_dim.x - iterator.item->item.rect.half_dim.x;
                           }
                           if(iterator.item->item.acceleration.y < 0){
                               iterator.item->item.rect.center.y = second_iterator.item->item.rect.center.y + second_iterator.item->item.rect.half_dim.y + iterator.item->item.rect.half_dim.y;
                           }
                           if(iterator.item->item.acceleration.y > 0){
                               iterator.item->item.rect.center.y = second_iterator.item->item.rect.center.y - second_iterator.item->item.rect.half_dim.y - iterator.item->item.rect.half_dim.y;
                           }
                           physics_collision collision = {body1,body2};
                           ARRAY_ADD(physics_collision,&data.current_collisions,collision);
                       }
                   }
               }
               if(collided){
                   iterator.item->item.acceleration = create_vec2(0.0,0.0);
               }
               if(data.flags & PHYS_DEBUG)
                   render_rect(iterator.item->item.rect,red);
           }else{
               if(data.flags & PHYS_DEBUG)
                   render_rect(iterator.item->item.rect,green);
           }
       }
   }
}
ARRAY(physics_collision) get_collisions(){
    return data.current_collisions;
}

*/
