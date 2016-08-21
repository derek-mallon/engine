#include <stdint.h>
#include "sdl_wrapper.h"
static color red = {1,0,0,1};
static color blue = {0,0,1,1};
static color green = {0,1,0,1};
typedef struct rect{
    vec2 center;
    vec2 half_dim;
}rect;
bool aabb(rect rect1,rect rect2){
    return (fabsf(rect1.center.x-rect2.center.x) < (rect1.half_dim.x + rect2.half_dim.y) && fabsf(rect1.center.y-rect2.center.y) < (rect1.half_dim.y + rect2.half_dim.y));
}
void render_rect(rect rect,color color){
    render_rect_outline(rect.center,scaler_multi(rect.half_dim,2),color);
}
void update_physics(){
    
}
int main(){
    init_sdl("test",1600,1000,NULL,0);
    sdl_layer_output output;
    rect rect1 = {create_vec2(0,0),create_vec2(0.5,0.5)};
    rect rect2 = {create_vec2(-4,0),create_vec2(0.5,0.5)};
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
