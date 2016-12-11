#include "engine.h"
sprite create_sprite(animation* animations,size_t number_of_animations){
    int i= 0;
    sprite output;
    output.animations = animations;
    output.current_animation = 0;
    output.angle = 0.0;
    vec2 rotation_point = {0,0};
    output.rotation_point = rotation_point;
    output.flip = F_NONE;
    output.number_of_animations = number_of_animations;
    return output;
}
typedef struct phys_body{
    rect dimensions;
}phys_body;
void destroy_sprite(sprite* sprite){
    int i=0;
    for(i=0;i<sprite->number_of_animations;i++){
        destroy_animation(&sprite->animations[i]);
    }
    free(sprite->animations);
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
void render_sprite(sprite* sprite,vec2 pos){
    animation* animation = &sprite->animations[sprite->current_animation];
    frame current_frame = animation->frames[animation->current_frame];
    render_frame(sprite->texture_index,current_frame,pos,sprite->rotation_point,sprite->angle,sprite->flip);
    if(animation->current_frame < animation->number_of_frames - 1){
        animation->current_frame++;
    }else{
        animation->current_frame = 0;
    }
}
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
    sprite test_sprite = create_sprite(animations,8);
    sdl_layer_output output= input_loop();
    pos = create_vec2(0,0);
    sprite_state current_state;
    while(get_running()){
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
        render_sprite(&test_sprite,pos);
        render_clear();
    }
    destroy_sprite(&test_sprite);
    quit();
}
