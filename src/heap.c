#include <stdlib.h>
#include <tester.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#ifdef DEBUG
size_t heap_counter = 0;
size_t numb_of_allocations = 0;
#endif

typedef struct heap_template{
    size_t size_of_object;
    size_t capacity;
    char name[256];
}heap_template;

typedef struct heap{
    size_t size_of_object;
    size_t capacity;
    void* ptr;
    uint8_t* alive;
    char name[256];
}heap;

typedef struct heap_manager{
    heap* heaps;
    size_t number_of_heaps;
}heap_manager;

static heap_manager manager;


uint8_t create_heap(size_t size_of_object,size_t capacity,heap* result,const char* name){
#ifdef DEBUG
    heap_counter++;
    numb_of_allocations++;
#endif
    void* ptr = malloc(size_of_object*capacity);
    size_t* next_free = malloc(sizeof(size_t)*capacity);
    int i;
    if(ptr == NULL || next_free == NULL)
        return 0;
    result->ptr = ptr;
    result->size_of_object = size_of_object;
    result->capacity = capacity;
    result->alive = malloc(sizeof(uint8_t)*capacity);
    strcpy(result->name,name);
    for(i=0;i<capacity;i++){
        result->alive[i] = 0;
    }
    return 1;
}

heap_template _create_heap_template(size_t object_size,size_t capacity,char* name){
    heap_template result = {object_size,capacity};
    strcpy(result.name,name);
    return result;
}

#define create_heap_template(type,capacity) _create_heap_template(sizeof(type),capacity,#type)

uint8_t destroy_heap(heap* h){
    if(h->capacity == 0)
        return 0;
#ifdef DEBUG
    heap_counter--;
#endif
    free(h->ptr);
    free(h->alive);
    h->capacity = 0;
    return 1;
}

const int line_size = 256;
heap_manager init_heap_manager(size_t count,heap_template*(*heap_init_func)(void)){

    FILE *data_file,*template_info_file,*template_data_file,*data,*alive_table;
    char buff[line_size];
    char* c;
    long number_of_templates;
    int i,j;
    char name_buff[line_size+10];
    void* alive;
    heap_template* templates = heap_init_func();
    heap_template* old_templates;
    heap* heaps;
    heap_manager manager;

    heaps = malloc(count*sizeof(heap));

    for(i=0;i<count;i++){
        create_heap(templates[i].size_of_object,templates[i].capacity,&heaps[i],templates[i].name);
    }

    if(access("templates.info",F_OK) != -1 && access("templates.data",F_OK)){

        template_info_file = fopen("templates.info","r");
        fgets(buff,line_size,template_info_file);
        number_of_templates = strtoll(buff,&c,10);
        fclose(template_info_file);

        template_data_file = fopen("templates.data","r");
        old_templates = malloc(sizeof(heap_template)*number_of_templates);
        fread(old_templates,sizeof(heap_template),number_of_templates,template_data_file);
        fclose(template_data_file);
        
        for(i=0;i<number_of_templates;i++){
            for(j=0;j<count;j++){
                if(strcmp(old_templates[i].name,templates[j].name) == 0){
                    if(old_templates[i].size_of_object == templates[j].size_of_object && old_templates[i].capacity == templates[j].capacity){

                        strcpy(name_buff,templates[j].name);
                        strcat(name_buff,".alive");

                        alive_table = fopen(name_buff,"r");

                        strcpy(name_buff,templates[j].name);
                        strcat(name_buff,".data");

                        data = fopen(name_buff,"r");

                        fread(heaps[j].ptr,heaps[j].size_of_object,heaps[j].capacity,data);
                        fclose(data);

                        fread(alive,sizeof(uint8_t),heaps[j].capacity,alive_table);
                        fclose(alive_table);
                        heaps[j].alive = alive;

                    }
                }
            }
        }
    }
    manager.heaps = heaps;
    manager.number_of_heaps = count;
    free(templates);
    return manager;
}

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

    template_info = fopen("templates.info","r");
    sprintf(buff,"%ld",manager->number_of_heaps);
    fputs(buff,template_info);
    fclose(template_info);
    free(templates);

    template_data = fopen("templates.data","r");
    fwrite(templates,sizeof(heap_template),manager->number_of_heaps,template_data);
    fclose(template_data);

    for(i=0;i<manager->number_of_heaps;i++){
        data = fopen(manager->heaps[i].name,"r");
        fwrite(manager->heaps[i].ptr,manager->heaps[i].size_of_object,manager->heaps[i].capacity,data);
        fclose(data);

        strcpy(buff,manager->heaps[i].name);
        strcat(buff,".alive");
        alive_table = fopen(buff,"r");
        fwrite(manager->heaps[i].alive,sizeof(uint8_t),manager->heaps[i].capacity,alive_table);
        fclose(alive_table);

    }

}
void destroy_heap_manager(heap_manager* manager){
    int i;
    for(i=0;i<manager->number_of_heaps;i++){
        free(manager->heaps[i].ptr);
        free(manager->heaps[i].alive);
    }
}

int main(void){
    heap int_heap;
    create_heap(sizeof(int),10,&int_heap,"test");  
    heap heap_heap;
    create_heap(sizeof(heap),10,&heap_heap,"test");  
    FILE* info_file = fopen("info","r+");
     
    destroy_heap(&int_heap);
    destroy_heap(&heap_heap);
    fclose(info_file);
}

