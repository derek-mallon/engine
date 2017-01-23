#include "mem.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "file_handling.h"
#ifdef DEBUG
size_t heap_counter = 0;
size_t numb_of_allocations = 0;
#endif

ERR_error MEM_create_heap(MEM_heap_template template,MEM_heap* result){
#ifdef DEBUG
    heap_counter++;
    numb_of_allocations++;
#endif
    void* ptr = malloc(template.size_of_object*template.capacity);
    int i;
    if(ptr == NULL)
        return ERR_MEM;
    result->ptr = ptr;
    result->size_of_object = template.size_of_object;
    result->capacity = template.capacity;
    result->top = 0;
    strcpy(result->name,template.name);
    return ERR_GOOD;
}
MEM_heap_template _MEM_create_heap_template(size_t object_size,size_t capacity,UTI_str name){
    MEM_heap_template result = {object_size,capacity};
    strcpy(result.name,name);
    return result;
}


ERR_error MEM_destroy_heap(MEM_heap* h){
    if(h->capacity == 0)
        return ERR_MEM;
#ifdef DEBUG
    heap_counter--;
#endif
    free(h->ptr);
    h->capacity = 0;
    return ERR_GOOD;
}

ERR_error MEM_create_heap_manager(UTI_str name,size_t count,void(*heap_init_func)(MEM_heap*,MEM_heap*),MEM_heap* init_data,MEM_heap_manager* manager){

    FILE *data_file,*template_info_file,*template_data_file,*data,*alive_table;
    char buff[UTI_DEFAULT_NAME_SIZE];
    char* c;
    long number_of_templates;
    int i,j;
    MEM_heap templates;
    MEM_create_heap(MEM_create_heap_template(MEM_heap_template,count),&templates);
    heap_init_func(&templates,init_data);
    MEM_heap* heaps;
    strcpy(manager->name,name);

    heaps = malloc(count*sizeof(MEM_heap));

    ERR_error result;
    for(i=0;i<count;i++){
        if((result = MEM_create_heap(MEM_get_item_m(MEM_heap_template,&templates,i),&heaps[i])) != ERR_GOOD){
            return result;
        }
    }

    MEM_destroy_heap(&templates);

    manager->heaps = heaps;
    manager->number_of_heaps = count;
    return ERR_GOOD;

}

ERR_error MEM_destroy_heap_manager(MEM_heap_manager* manager){
    int i;
    ERR_error result;
    for(i=0;i<manager->number_of_heaps;i++){
        if((result = MEM_destroy_heap(&manager->heaps[i])) != ERR_GOOD){
           return result;
        }
    }
    free(manager->heaps);
    return ERR_GOOD;
}



#define step_type(pos,type) pos += sizeof(type)

#define step_array(pos,inc,amount) pos += inc*amount

ERR_error MEM_add_top(MEM_heap* heap,size_t* index){
    if(heap->top >= heap->capacity){
        index = NULL;
        return ERR_MEM;
    }
    *index = heap->top;
    heap->top++;
    return ERR_GOOD;
}

size_t MEM_get_heap_binary_size(MEM_heap* heap){
    return (sizeof(size_t)*3+sizeof(heap->name)+heap->size_of_object*heap->capacity);
}

size_t MEM_get_heap_manager_binary_size(MEM_heap_manager* manager){
    int i;
    size_t size = sizeof(size_t) + sizeof(manager->name);
    for(i=0;i<manager->number_of_heaps;i++){
        size += MEM_get_heap_binary_size(&manager->heaps[i]);
    }
    return size;
}
void MEM_serialize_heap(MEM_heap* heap,size_t* pos,MEM_handle handle){
    void* data = &handle.heap->ptr[handle.index];
    *(size_t*)(&(data)[*pos]) = heap->size_of_object;
    step_type(*pos,size_t);
    *(size_t*)(&(data)[*pos]) = heap->capacity;
    step_type(*pos,size_t);
    *(size_t*)(&(data)[*pos]) = heap->top;
    step_type(*pos,size_t);
    strcpy((char*)(&(data)[*pos]),heap->name);
    step_type(*pos,heap->name);

    memcpy(&(data)[*pos],heap->ptr,(heap->size_of_object*heap->capacity));
    step_array(*pos,heap->size_of_object,heap->capacity);
}

void MEM_serialize_heap_manager(MEM_heap_manager* manager,size_t* pos,MEM_handle handle){
    void* data = &handle.heap->ptr[handle.index];
    int i;
    *(size_t*)(&(data)[0]) = manager->number_of_heaps;
    step_type(*pos,size_t);
    strcpy((char*)(&(data)[sizeof(size_t)]),manager->name);
    step_type(*pos,manager->name);

    for(i=0;i<manager->number_of_heaps;i++){
        MEM_serialize_heap(&manager->heaps[i],pos,handle);
    }
}

ERR_error MEM_deserialize_heap(MEM_heap* heap,size_t* pos,MEM_handle handle){
    void* data = &handle.heap->ptr[handle.index];
    heap->size_of_object = *(size_t*)(&data[*pos]);
    step_type(*pos,size_t);
    heap->capacity = *(size_t*)(&data[*pos]);
    step_type(*pos,size_t);
    heap->top = *(size_t*)(&data[*pos]);
    step_type(*pos,size_t);
    strcpy(heap->name,&data[*pos]);
    step_type(*pos,heap->name);

    heap->ptr = malloc(heap->capacity* heap->size_of_object);
    if(heap->ptr == NULL){
        return ERR_MEM;
    }

    memcpy(heap->ptr,&data[*pos],heap->capacity*heap->size_of_object);
    step_array(*pos,heap->size_of_object,heap->capacity);

    return ERR_GOOD;
}
ERR_error MEM_deserialize_heap_manager(MEM_heap_manager* manager,size_t* pos,MEM_handle handle){
    void* data = &handle.heap->ptr[handle.index];
    manager->number_of_heaps = *(size_t*)&data[*pos];
    step_type(*pos,size_t);
    strcpy(manager->name,(char*)&data[*pos]);
    step_type(*pos,manager->name);

    manager->heaps = malloc(sizeof(MEM_heap)*manager->number_of_heaps);
    if(manager->heaps == NULL){
        return ERR_MEM;
    }
    int i;
    for(i=0;i<manager->number_of_heaps;i++){
        MEM_deserialize_heap(&manager->heaps[i],pos,handle);
    }
    return ERR_GOOD;
}
MEM_handle MEM_create_handle_from_manager(MEM_heap_manager* manager,size_t index_of_heap,size_t index){
    ERR_ASSERT(manager->number_of_heaps > index_of_heap,"heap does not exist at the index %lu",index_of_heap)
    ERR_ASSERT(manager->heaps[index_of_heap].capacity > index,"item does not exit at the index %lu in the heap",index)
    MEM_handle handle = {&manager->heaps[index_of_heap],index};
    return handle;
}

MEM_handle MEM_create_handle_from_heap(MEM_heap* heap,size_t index){
    ERR_ASSERT(heap->capacity > index,"item does not exit at the index %lu in the heap",index);
    MEM_handle handle = {heap,index};
    return handle;
}
