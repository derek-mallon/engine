#ifndef SDL_WRAPPER_H
#define SDL_WRAPPER_H
#include "mem.h"
#include <SDL2/Sdl.h>
#include <stdint.h>
#include "geometry.h"
#include "utils.h"
#include "error.h"

#define EVENT_BUFF_SIZE 10
#define DELTA_SAMPLE_SIZE 60

typedef enum WPR_flip WPR_flip;

typedef struct WPR_sdl_data WPR_sdl_data;
typedef struct WPR_sdl_layer_output WPR_sdl_layer_output;
typedef struct WPR_init_sdl_data WPR_init_sdl_data;
typedef struct WPR_color WPR_color;
typedef  SDL_Texture* WPR_texture_ptr;

enum WPR_flip{
    F_NONE,
    F_HORTIZONATAL,
    F_VERTICAL,
};

struct WPR_sdl_data{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    uint16_t screen_height;
    uint16_t screen_width;
    float unit_x;
    float unit_y;
    float scale_x;
    float scale_y;
    float offset_x;
    float offset_y;
    uint8_t running;
    uint32_t prev_tick;
    uint32_t delta_time;
    uint32_t delta_time_data[DELTA_SAMPLE_SIZE];
    uint16_t delta_collection_count;
    float current_fps;
    MEM_heap* textures;

};

//Contains all of the output information
struct WPR_sdl_layer_output{
    SDL_Event current_event_buff[EVENT_BUFF_SIZE];
    size_t event_buff_size;
    uint64_t deltaTime;
    int mouse_x;
    int mouse_y;
};

struct WPR_init_sdl_data{
    char title[UTI_DEFAULT_NAME_SIZE];
    uint16_t screen_width;
    uint16_t screen_height;
};

struct WPR_color{
    float r,g,b,a;
};

float WPR_pixel_x_to_units(WPR_sdl_data* data,int pixel_x);

float WPR_pixel_y_to_units(WPR_sdl_data* data,int pixel_y);

int WPR_units_x_to_pixel(WPR_sdl_data* data,float units_x);

int WPR_units_y_to_pixel(WPR_sdl_data* data,float units_y);

void WPR_render_clear(WPR_sdl_data* data);

void WPR_render_texture(WPR_sdl_data* data,size_t texture_index,GEO_vec2 pos);

//NOTE rotation point is in pixels relative to the frame
void WPR_render_frame(WPR_sdl_data* data,size_t texture_index,uint16_t x,uint16_t y,uint16_t width,uint16_t height,GEO_vec2 pos,GEO_vec2 given_rotation_point,double angle,WPR_flip flip);

ERR_error WPR_init_sdl(WPR_sdl_data* data,WPR_init_sdl_data init,MEM_heap* textures);

WPR_sdl_layer_output WPR_input_loop(WPR_sdl_data* data);

uint8_t  WPR_get_running(WPR_sdl_data* data);

void WPR_quit(WPR_sdl_data* data);

WPR_color WPR_create_color(float r,float g,float  b,float a);

void WPR_render_rect_outline(WPR_sdl_data* data,GEO_vec2 pos,GEO_vec2 dim,WPR_color color);

void WPR_render_circle(WPR_sdl_data* data,GEO_vec2 pos,float radius,WPR_color color);

void WPR_update_timing(WPR_sdl_data* data);

uint32_t WPR_get_delta_time(WPR_sdl_data* data);

float WPR_get_fps(WPR_sdl_data* data);

ERR_error WPR_add_texture(WPR_sdl_data* data,SDL_Surface* surface,size_t* texture_index);

ERR_error WPR_cleanup(WPR_sdl_data* data);
#endif
