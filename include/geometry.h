#ifndef GEOMETRY_H
#define GEOMETRY_H

typedef struct GEO_vec2 GEO_vec2;
typedef struct GEO_rect GEO_rect;

struct GEO_vec2{
    float x;
    float y;
};

struct GEO_rect{
    GEO_vec2 center;
    GEO_vec2 half_dim;
};

GEO_vec2 GEO_create_vec2(float x,float y);
GEO_rect GEO_create_rect(float x,float y,float width,float height);
GEO_vec2 GEO_add_vec2(GEO_vec2 r,GEO_vec2 l);
GEO_vec2 GEO_sub_vec2(GEO_vec2 r,GEO_vec2 l);
GEO_vec2 GEO_multi_vec2(GEO_vec2 r,GEO_vec2 l);
GEO_vec2 GEO_scaler_vec2(GEO_vec2 vec,float s);
GEO_vec2 GEO_add_vec2_with_clamp(GEO_vec2 r,GEO_vec2 l,float clamp);
GEO_vec2 GEO_sub_vec2_with_clamp(GEO_vec2 r,GEO_vec2 l,float clamp);
GEO_vec2 GEO_approach_clamp_vec2(GEO_vec2 r,GEO_vec2 l,float clamp,float tolerance);
#endif
