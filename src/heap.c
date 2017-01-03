#include "heap.h"
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

uint8_t create_heap(heap_template template,heap* result){
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
    result->top = template.top;
    strcpy(result->name,template.name);
    return 1;
}

heap_template _create_heap_template(size_t object_size,size_t capacity,UTI_str name){
    heap_template result = {object_size,capacity};
    result.top = 0;
    strcpy(result.name,name);
    return result;
}

uint8_t destroy_heap(heap* h){
    if(h->capacity == 0)
        return 0;
#ifdef DEBUG
    heap_counter--;
#endif
    free(h->ptr);
    h->capacity = 0;
    return 1;
}

heap_manager init_heap_manager(UTI_str name,size_t count,void(*heap_init_func)(heap_template*)){

    FILE *data_file,*template_info_file,*template_data_file,*data,*alive_table;
    char buff[UTI_DEFAULT_NAME_SIZE];
    char* c;
    long number_of_templates;
    int i,j;
    void* alive;

    heap_template* templates = malloc(sizeof(heap_template)*count);
    heap_init_func(templates);
    heap* heaps;
    heap_manager manager;
    strcpy(manager.name,name);

    heaps = malloc(count*sizeof(heap));

    for(i=0;i<count;i++){
        create_heap(templates[i],&heaps[i]);
    }

    /*
    heap_template* old_templates;
    UTI_concat(buff,2,manager.name,"templates.data");
    if(access(buff,F_OK) != -1){

        template_data_file = fopen(buff,"rb");
        number_of_templates = FIL_file_size_binary(template_info_file)/sizeof(heap_template);
        old_templates = malloc(number_of_templates);
        fread(old_templates,sizeof(heap_template),number_of_templates,template_data_file);
        fclose(template_data_file);
        
        for(i=0;i<number_of_templates;i++){
            for(j=0;j<count;j++){
                if(strcmp(old_templates[i].name,templates[j].name) == 0){
                    if(old_templates[i].size_of_object == templates[j].size_of_object && old_templates[i].capacity == templates[j].capacity){
                        UTI_concat(buff,2,templates[j].name,".data");
                        if(access(buff,F_OK) != -1){
                            data = fopen(buff,"rb");
                            fread(heaps[j].ptr,heaps[j].size_of_object,heaps[j].capacity,data);
                            fclose(data);
                        }
                    }
                }
            }
        }
        free(old_templates);
    }
    */
    manager.heaps = heaps;
    manager.number_of_heaps = count;
    free(templates);
    return manager;

}

/*
void save_heap_manager(heap_manager* manager){
    FILE* template_info,*template_data,*data,*alive_table;
    int i;
    char buff[256];
    
    heap_template* templates = malloc(sizeof(heap_template)*manager->number_of_heaps);

    for(i=0;i<manager->number_of_heaps;i++){
        templates[i].size_of_object = manager->heaps[i].size_of_object;
        templates[i].capacity = manager->heaps[i].capacity;
        strcpy(templates[i].name, manager->heaps[i].name);
    }

    concat(buff,2,manager->name,"templates.data");
    template_data = fopen(buff,"w");
    sprintf(buff,"%ld",manager->number_of_heaps);
    fputs(buff,template_info);
    fclose(template_info);

    template_data = fopen("templates.data","w");
    fwrite(templates,sizeof(heap_template),manager->number_of_heaps,template_data);
    fclose(template_data);
    free(templates);

    for(i=0;i<manager->number_of_heaps;i++){

        concat(buff,2,manager->heaps[i].name,".data");
        data = fopen(buff,"w");
        fwrite(manager->heaps[i].ptr,manager->heaps[i].size_of_object,manager->heaps[i].capacity,data);
        fclose(data);


    }

}
*/
void destroy_heap_manager(heap_manager* manager){
    int i;
    for(i=0;i<manager->number_of_heaps;i++){
        free(manager->heaps[i].ptr);
    }
    free(manager->heaps);
}




