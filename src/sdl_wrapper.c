#include "sdl_wrapper.h"
#include "utils.h"
#include "all.h"

#define DELTA_SAMPLE_SIZE 60


static WPR_status WPR_STATUS = WPR_CLOSED;

const float IDEAL_WIDTH = 16;
const float IDEAL_HEIGHT = 10;
const float IDEAL_UNIT_X = 100;
const float IDEAL_UNIT_Y = 100;


ERR_error WPR_init_sdl(WPR_init_sdl_data init,MEM_heap_manager* manager){
    MEM_handle sdl_data_handle= MEM_create_handle_from_manager(manager,MEM_LOC_WPR_SDL_DATA,0);
    WPR_sdl_data* data = MEM_get_item_p(WPR_sdl_data,sdl_data_handle);
    int i,j;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't init SDL: %s",SDL_GetError());
        return ERR_BAD;
    }
    data->window = SDL_CreateWindow(init.title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,init.screen_width,init.screen_height,SDL_WINDOW_SHOWN);
    if(!data->window){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't create a window: %s",SDL_GetError());
        return ERR_BAD;
    }
    data->renderer = SDL_CreateRenderer(data->window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!data->renderer){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't create a renderer: %s",SDL_GetError());
        return ERR_BAD;
    }
    
    data->screen_width = init.screen_width;
    data->screen_height = init.screen_height;
    data->unit_x = init.screen_width/IDEAL_WIDTH;
    data->unit_y = -init.screen_height/IDEAL_HEIGHT;
    data->scale_x = data->unit_x / (IDEAL_UNIT_X);
    data->scale_y = -data->unit_y / (IDEAL_UNIT_Y);
    data->offset_x =  (IDEAL_WIDTH/2);
    data->offset_y =  -(IDEAL_HEIGHT/2);
    data->running = 1;
    data->delta_time = 1;
    for(i=0;i<DELTA_SAMPLE_SIZE;i++){
        data->delta_time_data[i] = 1;
    }
    data->delta_collection_count = 0;
    data->current_fps = 0;
    data->prev_tick = SDL_GetTicks();
    data->textures = MEM_get_heap(manager,MEM_LOC_TEXTURE_DATA);
    WPR_STATUS = WPR_READY;
    return ERR_GOOD;
}
WPR_sdl_layer_output WPR_input_loop(WPR_sdl_data* data){
    WPR_sdl_layer_output output;
    output.event_buff_size = 0;
    while(SDL_PollEvent(&data->event) &&output.event_buff_size < EVENT_BUFF_SIZE){
        if(data->event.type == SDL_QUIT){
            data->running = 0;
        }
        output.current_event_buff[output.event_buff_size] = data->event;
        output.event_buff_size++; 
    }
    SDL_GetMouseState(&output.mouse_x,&output.mouse_y);
    output.mouse_x /= data->scale_x;
    output.mouse_y /= data->scale_y;
    int i;
    return output;
}

float WPR_pixel_x_to_units(WPR_sdl_data* data,int pixel_x){
    return (pixel_x/data->unit_x  - data->offset_x);
}
float WPR_pixel_y_to_units(WPR_sdl_data* data,int pixel_y){
    return (pixel_y/data->unit_y  - data->offset_y);
}
int WPR_units_x_to_pixel(WPR_sdl_data* data,float units_x){
    return (units_x + data->offset_x)* data->unit_x;
}
int WPR_units_x_scalar_to_pixel(WPR_sdl_data* data,float units_x){
    return units_x * data->unit_x;
}
int WPR_units_y_scalar_to_pixel(WPR_sdl_data* data,float units_y){
    return units_y * data->unit_y;
}
int WPR_units_y_to_pixel(WPR_sdl_data* data,float units_y){
    return (units_y + data->offset_y)* data->unit_y;
}
void WPR_render_clear(WPR_sdl_data* data){
    SDL_RenderPresent(data->renderer);
    SDL_SetRenderDrawColor(data->renderer,0,0,0,0xFF);
    SDL_RenderClear(data->renderer);
}
void WPR_render_texture(WPR_sdl_data* data,size_t texture_index,GEO_vec2 pos){
    int width;
    int height;
    SDL_QueryTexture(MEM_get_item_m(WPR_texture_ptr,data->textures,texture_index),NULL,NULL,&width,&height);
    SDL_Rect pos_rect = {(WPR_units_x_to_pixel(data,pos.x)-width*data->unit_x/2),(WPR_units_y_to_pixel(data,pos.y)-height*data->unit_y/2),width,height};
    SDL_RenderCopy(data->renderer,MEM_get_item_m(WPR_texture_ptr,data->textures,texture_index),NULL,&pos_rect);
}
void WPR_render_frame(WPR_sdl_data* data,size_t texture_index,uint16_t x,uint16_t y,uint16_t width,uint16_t height,GEO_vec2 pos,GEO_vec2 given_rotation_point,double angle,WPR_flip flip){
    SDL_Rect source_rect = {x,y,width,height};
    SDL_Rect pos_rect = {(WPR_units_x_to_pixel(data,pos.x)-width*data->scale_x/2),(WPR_units_y_to_pixel(data,pos.y)-height*data->scale_y/2),width*data->scale_x,height*data->scale_y};
    SDL_Point rotation_point = {given_rotation_point.x,given_rotation_point.y};
    SDL_RenderCopyEx(data->renderer,MEM_get_item_m(WPR_texture_ptr,data->textures,texture_index),&source_rect,&pos_rect,angle,&rotation_point,(SDL_RendererFlip)flip);
}
WPR_color WPR_create_color(float r,float g,float  b,float a){
    WPR_color color = {r,g,b,a};
    return color;
}
uint8_t WPR_get_running(WPR_sdl_data* data){

    return data->running;
}
void WPR_render_rect_outline(WPR_sdl_data* data,GEO_vec2 pos,GEO_vec2 dim,WPR_color color){
    int width = WPR_units_x_scalar_to_pixel(data,dim.x);
    int height = WPR_units_y_scalar_to_pixel(data,dim.y);
    SDL_Rect rect = {WPR_units_x_to_pixel(data,pos.x)-width*data->unit_x/2,WPR_units_y_to_pixel(data,pos.y)-height*data->unit_y/2,width,height};
    SDL_SetRenderDrawColor(data->renderer,color.r*255,color.g*255,color.b*255,color.a*255);
    SDL_RenderDrawRect(data->renderer,&rect);
}
void WPR_render_circle(WPR_sdl_data* data,GEO_vec2 pos,float radius,WPR_color color){
    SDL_RenderSetScale(data->renderer,1,1);
    SDL_SetRenderDrawColor(data->renderer,color.r*255,color.g*255,color.b*255,color.a*255);
    SDL_RenderDrawPoint(data->renderer,WPR_units_x_to_pixel(data,pos.x),WPR_units_y_to_pixel(data,pos.y));
    SDL_RenderSetScale(data->renderer,data->scale_x,data->scale_y);
}
void WPR_update_timing(WPR_sdl_data* data){
    uint32_t current_tick = SDL_GetTicks();
    data->delta_time = current_tick - data->prev_tick;
    if(data->delta_collection_count >= DELTA_SAMPLE_SIZE){
        int i;
        uint32_t sum = 0;
        for(i=0;i<DELTA_SAMPLE_SIZE;i++)
            sum += data->delta_time_data[i];
        data->current_fps = (1/(((float)sum)/DELTA_SAMPLE_SIZE))*1000;
        printf("---------- \n");
        printf("delta %u \n",data->delta_time);
        printf("fps %f \n",data->current_fps);
        data->delta_collection_count = 0;
    }else{
        data->delta_time_data[data->delta_collection_count] = data->delta_time;
    }
    data->prev_tick = current_tick;
    data->delta_collection_count++;
}
uint32_t WPR_get_delta_time(WPR_sdl_data* data){
    return data->delta_time;
}

float WPR_get_fps(WPR_sdl_data* data){
    return data->current_fps;
}


ERR_error WPR_turn_surface_into_texture(WPR_sdl_data* data,SDL_Surface* surface,WPR_texture_ptr* texture){
    *texture = SDL_CreateTextureFromSurface(data->renderer,surface);
    if(*texture == NULL){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't turn the surface into a texture: %s",SDL_GetError());
        return ERR_BAD;
    }
    SDL_FreeSurface(surface);
    return ERR_GOOD;
}

ERR_error WPR_shutdown(WPR_sdl_data* data){
    ERR_ASSERT(AST_get_status() == AST_CLOSED,"assets not closed yet");
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
    data->renderer = NULL;
    data->window = NULL;
    SDL_Quit();
    WPR_STATUS = WPR_CLOSED;
    return ERR_GOOD;
}

WPR_status WPR_get_status(){
    return WPR_STATUS;
}

