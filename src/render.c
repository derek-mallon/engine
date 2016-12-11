#include "render.h"

ARRAY_DEF(frame)
ARRAY_DEF(animation)

animation create_animation_from_strip(size_t texture_index,uint16_t number_of_frames,uint16_t sprite_width,uint16_t sprite_height,uint16_t x_offset,uint16_t y_offset){
    int i= 0;
    animation output;
    output.texture_index = texture_index;
    output.frames = ARRAY_CREATE(frame,number_of_frames);
    for(i=0;i<number_of_frames;i++){
        frame x = {i*sprite_width + x_offset,y_offset,sprite_width,sprite_height};
        ARRAY_ADD(frame,&output.frames,x);
    }
    output.current_frame = 0;
    return output;
};

void destroy_animation(animation* animation){
    ARRAY_DESTROY(frame,&animation->frames);
}

sprite create_sprite(size_t number_of_animations,size_t texture_index){
    int i= 0;
    sprite output;
    output.animations = ARRAY_CREATE(animation,number_of_animations);
    output.current_animation = 0;
    output.angle = 0.0;
    vec2 rotation_point = {0,0};
    output.rotation_point = rotation_point;
    output.flip = F_NONE;
    output.texture_index = texture_index;
    return output;
}

void destroy_sprite(sprite* sprite){
    int i=0;
    for(i=0;i<sprite->animations.size;i++){
        destroy_animation(&sprite->animations.array[i]);
    }
    ARRAY_DESTROY(animation,&sprite->animations);
}

void step_animation_sprite(sprite* sprite){
    animation* animation = &sprite->animations.array[sprite->current_animation];
    if(animation->current_frame < animation->frames.size - 1){
        animation->current_frame++;
    }else{
        animation->current_frame = 0;
    }
}

void render_sprite(sprite* sprite,vec2 pos){
    animation* animation = &sprite->animations.array[sprite->current_animation];
    frame current_frame = animation->frames.array[animation->current_frame];
    render_frame(sprite->texture_index,current_frame.x,current_frame.y,current_frame.width,current_frame.height,pos,sprite->rotation_point,sprite->angle,sprite->flip);
}

int main(){
    char* paths[1];
    paths[0] = "../testsheet.png";
    init_sdl("test",1600,1000,paths,1);
    vec2 pos;
    sprite test_sprite = create_sprite(8,0);
    size_t WALKING_EAST = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,10,120,130,0,910));
    size_t WALKING_NORTH = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,10,120,130,0,780));
    size_t WALKING_WEST = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,10,120,130,0,650));
    size_t WALKING_SOUTH = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,10,120,130,0,520));
    size_t STANDING_EAST = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,3,120,130,0,390));
    size_t STANDING_NORTH = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,1,120,130,0,260));
    size_t STANDING_WEST = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,3,120,130,0,130));
    size_t STANDING_SOUTH = ARRAY_ADD(animation,&test_sprite.animations,create_animation_from_strip(0,3,120,130,0,0));
    sdl_layer_output output= input_loop();
    pos = create_vec2(0,0);
    size_t current_state;
    size_t counter = 0;
    while(get_running()){
        //INPUT
        output= input_loop();
        update_timing();
        /*
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
            if(current_state == WALKING_NORTH)
                current_state = STANDING_NORTH;
            if(current_state == WALKING_EAST)
                current_state = STANDING_EAST;
            if(current_state == WALKING_SOUTH)
                current_state = STANDING_SOUTH;
            if(current_state == WALKING_WEST)
                current_state = STANDING_WEST;
        //RENDER
        */
        counter += get_delta_time();
        printf("%lu\n",counter);
        if(counter >= 100){
            counter = 0;
            step_animation_sprite(&test_sprite);
        }
        render_sprite(&test_sprite,pos);
        render_clear();
    }
    destroy_sprite(&test_sprite);
    quit();
}
