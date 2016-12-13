#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include <SDL2/Sdl.h>
#include <stdint.h>
#include <stdbool.h>
#include <geometry.h>
#include <array.h>

#define EVENT_BUFF_SIZE 10

typedef char* string;

ARRAY_DEC(string)
//frame flip
typedef enum flip{
    F_NONE,
    F_HORTIZONATAL,
    F_VERTICAL,
}flip;

//Contains all of the output information
typedef struct sdl_layer_output{
    SDL_Event current_event_buff[EVENT_BUFF_SIZE];
    size_t event_buff_size;
    uint64_t deltaTime;
    int mouse_x;
    int mouse_y;
}sdl_layer_output;

typedef struct init_sdl_data{
    const char* title;
    uint16_t screen_width;
    uint16_t screen_height;
    ARRAY(string) texture_paths;
}init_sdl_data;


float pixel_x_to_units(int pixel_x);

float pixel_y_to_units(int pixel_y);

int units_x_to_pixel(float units_x);

int units_y_to_pixel(float units_y);

void render_clear();

void render_texture(size_t texture_index,vec2 pos);

//NOTE rotation point is in pixels relative to the frame
void render_frame(size_t texture_index,uint16_t x,uint16_t y,uint16_t width,uint16_t height,vec2 pos,vec2 given_rotation_point,double angle,flip flip);

void init_sdl(init_sdl_data data);
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
