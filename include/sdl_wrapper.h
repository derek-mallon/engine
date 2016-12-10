#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL2/Sdl.h>
#include <stdint.h>
#include <stdbool.h>
#include <geometry.h>

#define EVENT_BUFF_SIZE 10

//Contains all of the output information
typedef struct sdl_layer_output{
    SDL_Event current_event_buff[EVENT_BUFF_SIZE];
    size_t event_buff_size;
    uint64_t deltaTime;
    int mouse_x;
    int mouse_y;
}sdl_layer_output;

//Frame
typedef struct frame{
    uint16_t x,y,width,height;
}frame;

//Animation
typedef struct animation{
    frame* frames;
    size_t number_of_frames;
    size_t current_frame;
    size_t texture_index;
}animation;

//Create an animation from frames in a straight line from the given cords
animation create_animation_from_strip(size_t texture_index,uint16_t number_of_frames,uint16_t sprite_width,uint16_t sprite_height,uint16_t x_offset,uint16_t y_offset);

void destroy_animation(animation* animation);

//frame flip
typedef enum flip{
    F_NONE,
    F_HORTIZONATAL,
    F_VERTICAL,
}flip;

float pixel_x_to_units(int pixel_x);

float pixel_y_to_units(int pixel_y);

int units_x_to_pixel(float units_x);

int units_y_to_pixel(float units_y);

void render_clear();

void render_texture(size_t texture_index,vec2 pos);

//NOTE rotation point is in pixels relative to the frame
void render_frame(size_t texture_index,frame frame,vec2 pos,vec2 rotation_point,double angle,flip flip);

void init_sdl(char* title,uint16_t screen_width,uint16_t screen_height,char** texture_paths,size_t number_of_textures);
sdl_layer_output input_loop();

bool get_running();

void quit();

typedef struct color{
    float r,g,b,a;
}color;

color create_color(float r,float g,float  b,float a);

void render_rect_outline(vec2 pos,vec2 dim,color color);

vec2 scaler_multi(vec2 vec,float scaler);

void render_circle(vec2 pos,float radius,color color);

void update_timing();

uint32_t get_delta_time();

float get_fps();
#endif
