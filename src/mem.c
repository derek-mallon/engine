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

uint8_t MEM_create_heap(MEM_heap_template template,MEM_heap* result){
#ifdef DEBUG
    heap_counter++;
    numb_of_allocations++;
#endif
    void* ptr = malloc(template.size_of_object*template.capacity);
    int i;
    if(ptr == NULL)
        return 0;
    result->ptr = ptr;
    result->size_of_object = template.size_of_object;
    result->capacity = template.capacity;
    result->top = 0;
    result->alive_list = malloc(sizeof(size_t)*template.capacity);
    result->flags = malloc(sizeof(uint8_t)*template.capacity);
    result->alive_top  = 0;
    strcpy(result->name,template.name);
    return 1;
}
MEM_heap_template _MEM_create_heap_template(size_t object_size,size_t capacity,UTI_str name){
    MEM_heap_template result = {object_size,capacity};
    strcpy(result.name,name);
    return result;
}

uint8_t MEM_destroy_heap(MEM_heap* h){
    if(h->capacity == 0)
        return 0;
#ifdef DEBUG
    heap_counter--;
#endif
    free(h->ptr);
    h->capacity = 0;
    return 1;
}

MEM_heap_manager MEM_create_heap_manager(UTI_str name,size_t count,void(*heap_init_func)(MEM_heap_template*)){

    FILE *data_file,*template_info_file,*template_data_file,*data,*alive_table;
    char buff[UTI_DEFAULT_NAME_SIZE];
    char* c;
    long number_of_templates;
    int i,j;
    void* alive;

    MEM_heap_template* templates = malloc(sizeof(MEM_heap_template)*count);
    heap_init_func(templates);
    MEM_heap* heaps;
    MEM_heap_manager manager;
    strcpy(manager.name,name);

    heaps = malloc(count*sizeof(MEM_heap));

    for(i=0;i<count;i++){
        MEM_create_heap(templates[i],&heaps[i]);
    }

    manager.heaps = heaps;
    manager.number_of_heaps = count;
    free(templates);
    return manager;

}

void MEM_destroy_heap_manager(MEM_heap_manager* manager){
    int i;
    for(i=0;i<manager->number_of_heaps;i++){
        free(manager->heaps[i].ptr);
        free(manager->heaps[i].alive_list);
        free(manager->heaps[i].flags);
    }
    free(manager->heaps);
}

ERR_error MEM_free_item(MEM_heap* h,size_t index){
    if(h->flags[index] & MEM_DEAD){
        return ERR_BAD;
    }
    h->alive_list[h->alive_top] = index;
    h->flags[index] |= MEM_DEAD;
    h->alive_top++;
    return ERR_GOOD;
}

ERR_error MEM_next_free_item(MEM_heap* h,size_t* refrence){
    if(h->alive_top != 0){
        *refrence =  h->alive_list[h->alive_top -1];
        h->alive_top--;
        h->flags[*refrence] &=  ~MEM_DEAD;
    }else if(h->top < h->capacity-1){
        *refrence = h->top;
        h->top++;
    }else{
       return ERR_MEM;
    }
    return ERR_GOOD;
}


