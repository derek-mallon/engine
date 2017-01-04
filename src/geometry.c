#include "geometry.h"
GEO_vec2 GEO_create_vec2(float x,float y){
    GEO_vec2 vec = {x,y};
    return vec;
}
GEO_rect GEO_create_rect(float x,float y,float width,float height){
    GEO_rect output = {GEO_create_vec2(x,y),GEO_create_vec2(width/2,height/2)};
    return output;
}
GEO_vec2 GEO_add_vec2(GEO_vec2 r,GEO_vec2 l){
    GEO_vec2 result = {r.x+l.x,r.y+l.y};
    return result;
}
GEO_vec2 GEO_sub_vec2(GEO_vec2 r,GEO_vec2 l){
   GEO_vec2 result = {r.x-l.x,r.y-l.y};
    return result;
}
GEO_vec2 GEO_add_vec2_with_clamp(GEO_vec2 r,GEO_vec2 l,float clamp){
    GEO_vec2 result = {r.x+l.x,r.y+l.y};
    if(result.x > clamp)
        result.x = clamp;
    if(result.y > clamp)
        result.y = clamp;
    return result;
}
GEO_vec2 GEO_sub_vec2_with_clamp(GEO_vec2 r,GEO_vec2 l,float clamp){
    GEO_vec2 result = {r.x-l.x,r.y-l.y};
    if(result.x < clamp)
        result.x = clamp;
    if(result.y < clamp)
        result.y = clamp;
    return result;
}
GEO_vec2 GEO_approach_clamp_vec2(GEO_vec2 r,GEO_vec2 l,float clamp,float tolerance){
    GEO_vec2 result;
    if(r.x < clamp+tolerance && r.x > clamp-tolerance)
        result.x = clamp;
    else if(r.x > clamp)
        result.x = r.x - l.x;
    else if(r.x < clamp)
        result.x = r.x + l.x;

    if(r.y < clamp+tolerance && r.y > clamp-tolerance)
        result.y = clamp;
    else if(r.y > clamp)
        result.y = r.y - l.y;
    else if(r.y < clamp)
        result.y = r.y + l.y;

    return result;
}
GEO_vec2 GEO_multi_vec2(GEO_vec2 r,GEO_vec2 l){
    GEO_vec2 result = {r.x*l.x,r.y*l.y};
    return result;
}
GEO_vec2 GEO_scaler_vec2(GEO_vec2 vec,float s){
    GEO_vec2 result = {vec.x*s,vec.y*s};
    return result;
}
