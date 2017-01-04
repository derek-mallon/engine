#ifndef MEM_C
#define MEM_C
#include <stdlib.h>
#include "utils.h"
#include "error.h"

#define MEM_DEAD 0x01

typedef struct MEM_heap_template MEM_heap_template;
typedef struct MEM_heap MEM_heap;
typedef struct MEM_heap_manager MEM_heap_manager;
typedef struct MEM_chunk MEM_chunk;

struct MEM_heap_template{
    size_t size_of_object;
    size_t capacity;
    char name[UTI_DEFAULT_NAME_SIZE];
};

struct MEM_heap{
    size_t size_of_object;
    size_t capacity;
    void* ptr;
    size_t top;
    uint8_t* flags;
    size_t* alive_list;
    size_t alive_top;
    char name[UTI_DEFAULT_NAME_SIZE];
};

struct MEM_heap_manager{
    MEM_heap* heaps;
    size_t number_of_heaps;
    char name[UTI_DEFAULT_NAME_SIZE];
};

MEM_heap_template _MEM_create_heap_template(size_t object_size,size_t capacity,UTI_str name);

#define MEM_create_heap_template(type,capacity) _MEM_create_heap_template(sizeof(type),capacity,#type)

#define MEM_get_item(type,heap,i) (*(type*)&(heap)->ptr[i*((heap)->size_of_object)])

MEM_heap_manager MEM_create_heap_manager(UTI_str name,size_t count,void(*heap_init_func)(MEM_heap_template*));

void MEM_destroy_heap_manager(MEM_heap_manager* manager);

ERR_error MEM_free_item(MEM_heap* h,size_t index);

ERR_error MEM_next_free_item(MEM_heap* h,size_t* refrence);


#endif
