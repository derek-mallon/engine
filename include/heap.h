#ifndef HEAP_C
#define HEAP_C
#include <stdlib.h>
#include "utils.h"

typedef struct heap_template heap_template;
typedef struct heap heap;
typedef struct heap_manager heap_manager;

struct heap_template{
    size_t size_of_object;
    size_t capacity;
    size_t top;
    char name[UTI_DEFAULT_NAME_SIZE];
};

struct heap{
    size_t size_of_object;
    size_t capacity;
    void* ptr;
    size_t top;
    char name[UTI_DEFAULT_NAME_SIZE];
};

struct heap_manager{
    heap* heaps;
    size_t number_of_heaps;
    char name[UTI_DEFAULT_NAME_SIZE];
};



heap_template _create_heap_template(size_t object_size,size_t capacity,UTI_str name);

#define create_heap_template(type,capacity) _create_heap_template(sizeof(type),capacity,#type)

#define get_item(type,heap,i) *(type*)&heap->ptr[i]

heap_manager init_heap_manager(UTI_str name,size_t count,void(*heap_init_func)(heap_template*));

void save_heap_manager(heap_manager* manager);

void destroy_heap_manager(heap_manager* manager);
#endif
