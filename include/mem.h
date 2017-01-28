/**
 * @file mem.h
 * @author Derek Mallon
 * @brief Project wide memory managerment. Uses MEM_*
 * This provides the function definitions for manipulating heaps and heap managers which allow for all of the dynamic memory allocation to be done at once.
 * The heap data structure also provides memory recycling is needed.
 * The goal of the memory management is to prevent the need to allocate memory more than once during the life time of a program. 
 * Instead all of the memory is allocated in predefined heaps which serve as arrays of objects. 
 * Memory can also be saved and loaded from disk.
 */
#ifndef MEM_C
#define MEM_C
#include <stdlib.h>
#include "utils.h"
#include "error.h"

#define MEM_DEAD 0x01

typedef struct MEM_heap_template MEM_heap_template;
typedef struct MEM_heap MEM_heap;
typedef struct MEM_heap_manager MEM_heap_manager;
typedef struct MEM_handle MEM_handle;

/**
 * @brief template structure for creating heaps initially.
 * This only stores the properties of needed to create a heap.
 */
struct MEM_heap_template{
    size_t size_of_object;
    size_t capacity;
    char name[UTI_DEFAULT_NAME_SIZE];
};

/**
 * @brief heap structure.
 * Contains three heap allocated arrays. 
 * 1. ptr which is the objects themselves. 
 * 2. flags, these are any flags needed to describe the object at the same index in ptr.
 * 3. alive_list is a list of all of the freeded indexes. This makes it faster to reuse objects.
 * Each heap is given a name for debug purposes, when using a macro is set to the type of the heap
 */
struct MEM_heap{
    size_t size_of_object;
    size_t capacity;
    size_t top;
    void* ptr;
    char name[UTI_DEFAULT_NAME_SIZE];
};

/**
 * @brief heap manager structure used for creating, storing, and cleaning up heaps.
 * Contains an heap allocated array of heaps.
 * Has a name to identify it from other heap managers.
 */
struct MEM_heap_manager{
    MEM_heap* heaps;
    size_t number_of_heaps;
    char name[UTI_DEFAULT_NAME_SIZE];
};

struct MEM_handle{
    size_t index_of_heap;
    size_t index;
};

/**
 * @brief Internal function for creating heap templates, use the macro version.
 */
MEM_heap_template _MEM_create_heap_template(size_t object_size,size_t capacity,const char* name);

/**
 * @brief a macro which creates a heap template.
 * @param type pass the type of the object.
 * @param capacity the capacity of the heap.
 * @return a heap template.
 */
#define MEM_create_heap_template(type,capacity) _MEM_create_heap_template(sizeof(type),capacity,#type)

#define MEM_create_heap_template_not_type(size,capacity,name) _MEM_create_heap_template(size,capacity,name)


/**
 * @brief 
 */

ERR_error MEM_create_heap(MEM_heap_template template,MEM_heap* result);

ERR_error MEM_destroy_heap(MEM_heap* h);

ERR_error MEM_create_heap_manager(const char* name,size_t count,void(*heap_init_func)(MEM_heap*,void*),void* data,MEM_heap_manager* manager);

ERR_error MEM_destroy_heap_manager(MEM_heap_manager* manager);

ERR_error MEM_add_top(MEM_heap* heap,size_t* index);

size_t MEM_get_heap_binary_size(MEM_heap* heap);

size_t MEM_get_heap_manager_binary_size(MEM_heap_manager* manager);

void MEM_serialize_heap(MEM_heap* heap,size_t* pos,MEM_heap* binary);

void MEM_serialize_heap_manager(MEM_heap_manager* manager,size_t* total_size,MEM_heap* binary);

ERR_error MEM_deserialize_heap(MEM_heap* heap,size_t* pos,MEM_heap* binary);

ERR_error MEM_deserialize_heap_manager(MEM_heap_manager* manager,size_t* pos,MEM_heap* binary);

MEM_handle MEM_create_handle(size_t index_of_heap,size_t index);

MEM_heap* MEM_get_heap(MEM_handle handle,MEM_heap_manager* manager);

MEM_heap* MEM_get_heap_m(MEM_heap_manager* manager,size_t index);

ERR_error MEM_load_heap_binary(const char*,MEM_heap* heap);

ERR_error MEM_save_heap_binary(const char*,MEM_heap* heap);

ERR_error MEM_load_manager_binary(const char*,MEM_heap_manager* manager);

ERR_error MEM_save_manager_binary(const char*,MEM_heap_manager* manager);


/**
 * @brief a macro for getting an object with a given index.
 * @param type pass the type of the object (used for the casting).
 * @param heap pass a pointer to the heap which the object is located in.
 * @param i the index which will returned.
 * @return the object (not a copy).
 */


#define MEM_get_item(type,handle,manager) (*(type*)&(MEM_get_heap(handle,manager))->ptr[handle.index*(MEM_get_heap(handle,manager)->size_of_object)])

#define MEM_get_item_p(type,handle,manager) ((type*)&(MEM_get_heap(handle,manager))->ptr[handle.index*((MEM_get_heap(handle,manager))->size_of_object)])

#define MEM_get_item_m(type,heap,i) (*(type*)&(heap)->ptr[i*((heap)->size_of_object)])

#define MEM_get_item_m_p(type,heap,i) ((type*)&(heap)->ptr[i*((heap)->size_of_object)])


#endif
