#ifndef SYSTEM_H
#define SYSTEM_H
#include <objectpool.h>
#include <array.h>


#define SYSTEM_DEC(type)\
typedef struct type##_handle{\
    size_t index;\
    size_t entity_index;\
}type##_handle;\
typedef struct type##_component{\
    type item;\
    type##_handle handle;\
}type##_component;\
POOL_DEC(type##_component)\
typedef struct type##_system{\
    POOL(type##_component) components;\
}type##_system;\
void type##_system_init(size_t cap);\
type##_component* add_##type(type item,size_t entity_index);\
void free_##type(type##_component* item);\
type##_component* get_##type(type##_handle handle);\
void type##_system_destroy();

#define SYSTEM_DEF(type)\
POOL_DEF(type##_component)\
static type##_system system_;\
void type##_system_init(size_t cap){\
    system_.components = POOL_CREATE(type##_component,cap);\
}\
type##_component* add_##type(type item,size_t entity_index){\
    type##_component output;\
    output.item = item;\
    output.handle.entity_index = entity_index;\
    output.handle.index = system_.components.array.size;\
    type##_component* output_ptr = NULL;\
    if((output_ptr = POOL_NEXT_FREE(type##_component,&system_.components,&output.handle.index)))\
        *output_ptr = output;\
    else{\
        POOL_ADD(type##_component,&system_.components,output);\
        output_ptr = POOL_GET(type##_component,&system_.components,output.handle.index);\
    }\
    return output_ptr;\
}\
void free_##type(type##_component* component){\
    POOL_FREE(type##_component,&system_.components,component->handle.index);\
}\
type##_component* get_##type(type##_handle handle){\
    return POOL_GET(type##_component,&system_.components,handle.index);\
}\
void type##_system_destroy(){\
    POOL_DESTROY(type##_component,&system_.components);\
}\

#define SYSTEM_L_DEC(type)\
typedef struct type##_handle{\
    size_t index;\
    size_t layer;\
    size_t entity_index;\
}type##_handle;\
typedef struct type##_component{\
    type item;\
    type##_handle handle;\
}type##_component;\
POOL_DEC(type##_component)\
typedef struct type##_layer{\
    POOL(type##_component) components;\
}type##_layer;\
ARRAY_DEC(type##_layer)\
typedef struct type##_system{\
    ARRAY(type##_layer) layers;\
}type##_system;\
void type##_system_init(size_t cap,size_t layer_count);\
type##_component* add_##type(type item,size_t entity_index,size_t layer_index);\
void free_##type(type##_component* item);\
type##_component* get_##type(type##_handle handle);\
void type##_system_destroy();

#define SYSTEM_L_DEF(type)\
POOL_DEF(type##_component)\
ARRAY_DEF(type##_layer)\
static type##_system system_;\
void type##_system_init(size_t cap,size_t layer_count){\
    system_.layers = ARRAY_CREATE(type##_layer,layer_count);\
    int i;\
    for(i=0;i<layer_count;i++){\
        type##_layer layer;\
        layer.components = POOL_CREATE(type##_component,cap);\
        ARRAY_ADD(type##_layer,&system_.layers,layer);\
    }\
}\
type##_component* add_##type(type item,size_t entity_index,size_t layer_index){\
    if(layer_index >= system_.layers.size){\
        type##_layer layer;\
        layer.components = POOL_CREATE(type##_component,1);\
        layer_index = ARRAY_ADD(type##_layer,&system_.layers,layer);\
    }\
    type##_component output;\
    output.item = item;\
    output.handle.entity_index = entity_index;\
    output.handle.index = system_.layers.array[layer_index].components.array.size;\
    output.handle.layer = layer_index;\
    type##_component* output_ptr = NULL;\
    if((output_ptr = POOL_NEXT_FREE(type##_component,&system_.layers.array[layer_index].components,&output.handle.index)))\
        *output_ptr = output;\
    else{\
        POOL_ADD(type##_component,&system_.layers.array[layer_index].components,output);\
        output_ptr = POOL_GET(type##_component,&system_.layers.array[layer_index].components,output.handle.index);\
    }\
    return output_ptr;\
}\
void free_##type(type##_component* component){\
    POOL_FREE(type##_component,&system_.layers.array[component->handle.layer].components,component->handle.index);\
}\
type##_component* get_##type(type##_handle handle){\
    return POOL_GET(type##_component,&system_.layers.array[handle.layer].components,handle.index);\
}\
void type##_system_destroy(){\
    int i;\
    for(i=0;i<system_.layers.size;i++){\
        POOL_DESTROY(type##_component,&system_.layers.array[i].components);\
    }\
    ARRAY_DESTROY(type##_layer,&system_.layers);\
}\

#endif
