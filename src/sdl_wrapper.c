#include "sdl_wrapper.h"
#include <SDL2/SDL_image.h>
#define DELTA_SAMPLE_SIZE 60
typedef struct sdl_data{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture** textures;
    size_t number_of_textures;
    SDL_Event event;
    uint16_t screen_height;
    uint16_t screen_width;
    float unit_x;
    float unit_y;
    float scale_x;
    float scale_y;
    float offset_x;
    float offset_y;
    bool running;
    uint32_t prev_tick;
    uint32_t delta_time;
    uint32_t delta_time_data[DELTA_SAMPLE_SIZE];
    uint16_t delta_collection_count;
    float current_fps;
}sdl_data;

static sdl_data data;

vec2 create_vec2(float x,float y){
    vec2 vec = {x,y};
    return vec;
}

const float IDEAL_WIDTH = 16;
const float IDEAL_HEIGHT = 10;
const float IDEAL_UNIT_X = 100;
const float IDEAL_UNIT_Y = 100;

bool load_texture_from_image(char* path,SDL_Texture** texture){
    SDL_Surface* image_surface = IMG_Load(path);
    if(!image_surface){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't load the image at %s: %s",path,SDL_GetError());
        return false;
    }
    *texture = SDL_CreateTextureFromSurface(data.renderer,image_surface);
    if(!texture){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't turn the image at %s into a texture: %s",path,SDL_GetError());
        return false;
    }
    SDL_FreeSurface(image_surface);
    return true;
}
void quit(){
    int i=0;
    for(i=0;i<data.number_of_textures;i++){
        SDL_DestroyTexture(data.textures[0]);
    }
    SDL_DestroyRenderer(data.renderer);
    SDL_DestroyWindow(data.window);
    SDL_Quit();
    free(data.textures);
    exit(0);
}

void init_sdl(char* title,uint16_t screen_width,uint16_t screen_height,char** texture_paths,size_t number_of_textures){
    int i = 0;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't init SDL: %s",SDL_GetError());
        quit();
    }
    data.window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screen_width,screen_height,SDL_WINDOW_SHOWN);
    if(!data.window){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't create a window: %s",SDL_GetError());
        quit();
    }
    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't init SDL_image: %s",SDL_GetError());
        quit();
    }
    data.renderer = SDL_CreateRenderer(data.window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!data.renderer){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't create a renderer: %s",SDL_GetError());
        quit();
    }
    data.textures = malloc(sizeof(SDL_Texture*)*(number_of_textures));
    data.number_of_textures = number_of_textures;
    for(i=0;i<number_of_textures;i++){
        if(!load_texture_from_image(texture_paths[i],&data.textures[i])){
            quit();
        }
    }
    data.screen_width = screen_width;
    data.screen_height = screen_height;
    data.unit_x = screen_width/IDEAL_WIDTH;
    data.unit_y = -screen_height/IDEAL_HEIGHT;
    data.scale_x = data.unit_x / (IDEAL_UNIT_X);
    data.scale_y = -data.unit_y / (IDEAL_UNIT_Y);
    data.offset_x =  (IDEAL_WIDTH/2);
    data.offset_y =  -(IDEAL_HEIGHT/2);
    data.running = true;
    data.delta_time = 1;
    for(i=0;i<DELTA_SAMPLE_SIZE;i++){
        data.delta_time_data[i] = 1;
    }
    data.delta_collection_count = 0;
    data.current_fps = 0;
    data.prev_tick = SDL_GetTicks();
    SDL_RenderSetScale(data.renderer,data.scale_x,data.scale_y);
}
sdl_layer_output input_loop(){
    sdl_layer_output output;
    output.event_buff_size = 0;
    while(SDL_PollEvent(&data.event) &&output.event_buff_size < EVENT_BUFF_SIZE){
        if(data.event.type == SDL_QUIT){
            quit();
        }
        output.current_event_buff[output.event_buff_size] = data.event;
        output.event_buff_size++; 
    }
    SDL_GetMouseState(&output.mouse_x,&output.mouse_y);
    output.mouse_x /= data.scale_x;
    output.mouse_y /= data.scale_y;
    return output;
}

float pixel_x_to_units(int pixel_x){
    return (pixel_x/data.unit_x  - data.offset_x);
}
float pixel_y_to_units(int pixel_y){
    return (pixel_y/data.unit_y  - data.offset_y);
}
int units_x_to_pixel(float units_x){
    return (units_x + data.offset_x)* data.unit_x;
}
int units_x_scalar_to_pixel(float units_x){
    return units_x * data.unit_x;
}
int units_y_scalar_to_pixel(float units_y){
    return units_y * data.unit_y;
}
int units_y_to_pixel(float units_y){
    return (units_y + data.offset_y)* data.unit_y;
}
void render_clear(){
    SDL_RenderPresent(data.renderer);
    SDL_SetRenderDrawColor(data.renderer,0,0,0,0xFF);
    SDL_RenderClear(data.renderer);
}
void render_texture(size_t texture_index,vec2 pos){
    int width;
    int height;
    SDL_QueryTexture(data.textures[texture_index],NULL,NULL,&width,&height);
    SDL_Rect pos_rect = {(units_x_to_pixel(pos.x)-width/2),(units_y_to_pixel(pos.y)-height/2),width,height};
    SDL_RenderCopy(data.renderer,data.textures[texture_index],NULL,&pos_rect);
}
void render_frame(size_t texture_index,uint16_t x,uint16_t y,uint16_t width,uint16_t height,vec2 pos,vec2 given_rotation_point,double angle,flip flip){
    SDL_Rect source_rect = {x,y,width,height};
    SDL_Rect pos_rect = {(units_x_to_pixel(pos.x)-width/2),(units_y_to_pixel(pos.y)-height/2),width,height};
    SDL_Point rotation_point = {given_rotation_point.x,given_rotation_point.y};
    SDL_RenderCopyEx(data.renderer,data.textures[texture_index],&source_rect,&pos_rect,angle,&rotation_point,(SDL_RendererFlip)flip);
}
color create_color(float r,float g,float  b,float a){
    color color = {r,g,b,a};
    return color;
}
bool get_running(){

    return data.running;
}
void render_rect_outline(vec2 pos,vec2 dim,color color){
    int width = units_x_scalar_to_pixel(dim.x);
    int height = units_y_scalar_to_pixel(dim.y);
    SDL_Rect rect = {units_x_to_pixel(pos.x)-width/2,units_y_to_pixel(pos.y)-height/2,width,height};
    SDL_SetRenderDrawColor(data.renderer,color.r*255,color.g*255,color.b*255,color.a*255);
    SDL_RenderDrawRect(data.renderer,&rect);
}
void render_circle(vec2 pos,float radius,color color){
    SDL_RenderSetScale(data.renderer,1,1);
    SDL_SetRenderDrawColor(data.renderer,color.r*255,color.g*255,color.b*255,color.a*255);
    SDL_RenderDrawPoint(data.renderer,units_x_to_pixel(pos.x),units_y_to_pixel(pos.y));
    SDL_RenderSetScale(data.renderer,data.scale_x,data.scale_y);
}
vec2 scaler_multi(vec2 vec,float scaler){
    vec.x *= scaler;
    vec.y *= scaler;
    return vec;
}
void update_timing(){
    uint32_t current_tick = SDL_GetTicks();
    data.delta_time = current_tick - data.prev_tick;
    if(data.delta_collection_count >= DELTA_SAMPLE_SIZE){
        int i;
        uint32_t sum = 0;
        for(i=0;i<DELTA_SAMPLE_SIZE;i++)
            sum += data.delta_time_data[i];
        data.current_fps = (1/(((float)sum)/DELTA_SAMPLE_SIZE))*1000;
        printf("---------- \n");
        printf("delta %u \n",data.delta_time);
        printf("fps %f \n",data.current_fps);
        data.delta_collection_count = 0;
    }else{
        data.delta_time_data[data.delta_collection_count] = data.delta_time;
    }
    data.prev_tick = current_tick;
    data.delta_collection_count++;
}
uint32_t get_delta_time(){
    return data.delta_time;
}

float get_fps(){
    return data.current_fps;
}
