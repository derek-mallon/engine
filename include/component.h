#ifndef COMPONENT_H
#define COMPONENT_H
#include "all.h"
#include "mem.h"

typedef void (*COM_init_func)(MEM_handle handle,MEM_handle messanger);
typedef void (*COM_update_func)(MEM_handle handle,MEM_handle messanger,size_t dt);
typedef void (*COM_destroy_func)(MEM_handle handle,MEM_handle messanger);
typedef size_t (*COM_get_size_func)();
typedef size_t (*COM_get_number_of_messages)();
typedef char** (*COM_get_messages)();

typedef struct COM_component COM_component;
typedef struct COM_data COM_data;
typedef struct COM_component_mem_template COM_component_mem_template;


struct COM_component_mem_template{
    MEM_heap_template template;
    size_t number_of_messages;
};
struct COM_component{
    COM_init_func init_func;
    COM_update_func update_func;
    COM_destroy_func destroy_func;
    LIB_HANDLE lib;
};

#endif
