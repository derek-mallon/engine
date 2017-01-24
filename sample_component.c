#include "mem.h"
#include "messaging.h"

#define FOREACH_MESSAGE(make_message)\
    make_message(take_damage)\
    make_message(heal)\

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,


typedef enum HEALTH_MESSAGES{
    FOREACH_MESSAGE(GENERATE_ENUM)
}HEALTH_MESSAGES;
static const char* messages[]= {
    FOREACH_MESSAGE(GENERATE_STRING)
};

typedef struct health{
    float amount;
    uint32_t conditions;
}health;

//SET UP CODE
size_t get_size(){
    return sizeof(health);
}

size_t get_number_of_messages(){
    return 2;
}

size_t get_capacity(){
    return 100;
}


char** get_messages(){
    return (char**)messages;
}

//RUN TIME CODE
void init(MEM_handle handle,MEM_handle messanger){
    health* self= MEM_get_item_p(health,handle);
}

void update(MEM_handle handle,MES_messanger* messanger,size_t dt){

    health* self= MEM_get_item_p(health,handle);

}

void destroy(MEM_handle handle,MES_messanger* messanger){
    health* self= MEM_get_item_p(health,handle);
}

void set_attr(MEM_handle handle,MEM_handle data,uint32_t type){
    health* self= MEM_get_item_p(health,handle);
    switch(type){
        case 0: 
            self->amount = MEM_get_item(float,data);
            break;
    }
}

void take_damage_func(MEM_handle handle,MEM_handle data,MES_messanger* messanger){
    health* self= MEM_get_item_p(health,handle);
    self->amount =- MEM_get_item(float,data);
    if(self->amount < 0){
        
    }
}

void heal_func(MEM_handle handle,MEM_handle data,MES_messanger* messanger){
    health* self= MEM_get_item_p(health,handle);
}
