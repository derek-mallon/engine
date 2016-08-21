
#include <SDL2/Sdl.h>
#include <SDL2/Sdl_image.h>
#include <stdint.h>
typedef enum {false,true} bool;
typedef struct sdl_data{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture** textures;
    size_t number_of_textures;
    SDL_Event event;
    uint16_t screen_width;
    uint16_t screen_height;
    float unit_x;
    float unit_y;
    float scale_x;
    float scale_y;
    float offset_x;
    float offset_y;
    bool running;
}sdl_data;

sdl_data data;

typedef struct vec2{
    float x;
    float y;
}vec2;
vec2 create_vec2(float x,float y){
    vec2 vec = {x,y};
    return vec;
}
const size_t EVENT_BUFF_SIZE = 10;
const float IDEAL_WIDTH = 16;
const float IDEAL_HEIGHT = 10;
const float IDEAL_UNIT_X = 100;
const float IDEAL_UNIT_Y = 100;

typedef struct sdl_layer_output{
    SDL_Event current_event_buff[EVENT_BUFF_SIZE];
    size_t event_buff_size;
    uint64_t deltaTime;
    int mouse_x;
    int mouse_y;
}sdl_layer_output;
typedef struct frame{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
}frame;
typedef enum sprite_flip{
    SF_NONE,
    SF_HORTIZONATAL,
    SF_VERTICAL,
}sprite_flip;
typedef struct animation{
    frame* frames;
    size_t number_of_frames;
    size_t current_frame;
    size_t texture_index;
}animation;
animation create_animation_from_strip(size_t texture_index,uint16_t number_of_frames,uint16_t sprite_width,uint16_t sprite_height,uint16_t x_offset,uint16_t y_offset){
    int i= 0;
    animation output;
    output.texture_index = texture_index;
    output.frames = malloc(sizeof(frame)* number_of_frames);
    for(i=0;i<number_of_frames;i++){
        frame x = {i*sprite_width + x_offset,y_offset,sprite_width,sprite_height};
        output.frames[i] = x;
    }
    output.number_of_frames = number_of_frames;
    output.current_frame = 0;
    return output;
};
void destroy_animation(animation* animation){
    free(animation->frames);
}
typedef struct sprite{
    size_t texture_index;
    double angle;
    animation* animations;
    size_t current_animation;
    vec2 rotation_point;
    sprite_flip flip;
}sprite;
sprite create_sprite(animation* animations){
    int i= 0;
    sprite output;
    output.animations = animations;
    output.current_animation = 0;
    output.angle = 0.0;
    vec2 rotation_point = {0,0};
    output.rotation_point = rotation_point;
    output.flip = SF_NONE;
    return output;
}
void destroy_sprite(sprite* sprite){
    free(sprite->animations);
}
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
    data.renderer = SDL_CreateRenderer(data.window,-1,SDL_RENDERER_ACCELERATED);
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
int units_y_to_pixel(float units_y){
    return (units_y + data.offset_y)* data.unit_y;
}
void render_clear(){
    SDL_RenderPresent(data.renderer);
    SDL_RenderClear(data.renderer);
}
void render_texture(size_t texture_index,vec2 pos){
    int width;
    int height;
    SDL_QueryTexture(data.textures[texture_index],NULL,NULL,&width,&height);
    SDL_Rect pos_rect = {(units_x_to_pixel(pos.x)-width/2),(units_y_to_pixel(pos.y)-height/2),width,height};
    SDL_RenderCopy(data.renderer,data.textures[texture_index],NULL,&pos_rect);
}
void render_sprite(sprite* sprite,vec2 pos){
    size_t current_animation = sprite->current_animation;
    animation* animation = &sprite->animations[sprite->current_animation];
    frame current_frame = animation->frames[animation->current_frame];
    SDL_Rect source_rect = {current_frame.x,current_frame.y,current_frame.width,current_frame.height};
    SDL_Rect pos_rect = {(units_x_to_pixel(pos.x)-current_frame.width/2),(units_y_to_pixel(pos.y)-current_frame.height/2),current_frame.width,current_frame.height};
    SDL_Point rotation_point = {sprite->rotation_point.x,sprite->rotation_point.y};
    SDL_RenderCopyEx(data.renderer,data.textures[sprite->texture_index],&source_rect,&pos_rect,sprite->angle,&rotation_point,sprite->flip);
    if(animation->current_frame < animation->number_of_frames-1){
        animation->current_frame++;
    }else{
        animation->current_frame = 0;
    }
}
typedef enum sprite_state{
    WALKING_NORTH,
    WALKING_EAST,
    WALKING_SOUTH,
    WALKING_WEST,
    STANDING_NORTH,
    STANDING_EAST,
    STANDING_SOUTH,
    STANDING_WEST,
}sprite_state;
int main(){
    char* paths[1];
    paths[0] = "testsheet.png";
    init_sdl("test",1600,1000,paths,1);
    vec2 pos;
    animation* animations = malloc(sizeof(animation)*8);
    animations[WALKING_EAST] = create_animation_from_strip(0,10,120,130,0,910);
    animations[WALKING_NORTH] = create_animation_from_strip(0,10,120,130,0,780);
    animations[WALKING_WEST] = create_animation_from_strip(0,10,120,130,0,650);
    animations[WALKING_SOUTH] = create_animation_from_strip(0,10,120,130,0,520);
    animations[STANDING_EAST] = create_animation_from_strip(0,3,120,130,0,390);
    animations[STANDING_NORTH] = create_animation_from_strip(0,1,120,130,0,260);
    animations[STANDING_WEST] = create_animation_from_strip(0,3,120,130,0,130);
    animations[STANDING_SOUTH] = create_animation_from_strip(0,3,120,130,0,0);
    sprite test_sprite = create_sprite(animations);
    sdl_layer_output output= input_loop();
    pos = create_vec2(0,0);
    sprite_state current_state;
    while(data.running){
        //INPUT
        output= input_loop();
        int i=0;
        for(i=0;i<output.event_buff_size;i++){
            switch(output.current_event_buff[i].type){
                case SDL_KEYDOWN:
                    switch(output.current_event_buff[i].key.keysym.sym){
                        case SDLK_w:
                            current_state = WALKING_NORTH;
                            break;
                        case SDLK_d:
                            current_state = WALKING_EAST;
                            break;
                        case SDLK_s:
                            current_state = WALKING_SOUTH;
                            break;
                        case SDLK_a:
                            current_state = WALKING_WEST;
                            break;
                    }
                    break;
            }
        }

        //LOGIC
        test_sprite.current_animation = current_state;
        switch(current_state) {
            case WALKING_NORTH:
                current_state = STANDING_NORTH;
                break;
            case WALKING_EAST:
                current_state = STANDING_EAST;
                break;
            case WALKING_SOUTH:
                current_state = STANDING_SOUTH;
                break;
            case WALKING_WEST:
                current_state = STANDING_WEST;
                break;
            case STANDING_NORTH:
                break;
            case STANDING_EAST:
                break;
            case STANDING_SOUTH:
                break;
            case STANDING_WEST:
                break;
        }
        //RENDER
        char buff[256];
        sprintf(buff,"%f,%f",pixel_x_to_units(output.mouse_x),pixel_y_to_units(output.mouse_y));
        SDL_SetWindowTitle(data.window,buff);
        render_sprite(&test_sprite,pos);
        render_clear();
        SDL_Delay(100);
    }
    destroy_sprite(&test_sprite);
    quit();
}
