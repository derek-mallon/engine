#include "tester.h"
#include "mem.h"
#include <string.h>

MEM_heap_template test_heap_template;
MEM_heap test_heap;
MEM_heap_manager test_heap_manager;
MEM_handle handle;
MEM_heap mem_heap;


char buff[50];

enum{
    INT
};
void test_init_fun(MEM_heap* templates,void* data){
    MEM_get_item_m(MEM_heap_template,templates,INT) = MEM_create_heap_template(int,100);
}

void add_int(){
        size_t index;
        MEM_add_top(&test_heap_manager.heaps[INT],&index);
        MEM_handle handle = MEM_create_handle(INT,index);
        MEM_get_item(int,handle,&test_heap_manager) = 5;
}

ENVIROMENT_SETUP{ //Run before each unit test.
    MEM_heap empty;
    MEM_create_heap_manager("test manager",1,test_init_fun,&empty,&test_heap_manager);
}

ENVIROMENT_CLEANUP{ //Run after each unit test.
    if(test_heap_manager.number_of_heaps != 0){
        MEM_destroy_heap_manager(&test_heap_manager);
    }
}

TESTS

    size_t index;

    UNIT_TEST_START("create a heap template")
        test_heap_template = MEM_create_heap_template(int,100);
        ASSERT(test_heap_template.size_of_object == sizeof(int));
        ASSERT(test_heap_template.capacity == 100);
        ASSERT(strcmp(test_heap_template.name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("create heap_manager with one template")
        ASSERT(test_heap_manager.number_of_heaps == 1);
        ASSERT(test_heap_manager.heaps[INT].size_of_object == sizeof(int));
        ASSERT(test_heap_manager.heaps[INT].capacity == 100);
        ASSERT(test_heap_manager.heaps[INT].ptr != NULL);
        ASSERT(test_heap_manager.heaps[INT].top == 0);
        ASSERT(strcmp(test_heap_manager.heaps[INT].name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("assigning a value")
        handle = MEM_create_handle(INT,index);
        MEM_get_item(int,handle,&test_heap_manager) = 5;
        ASSERT(MEM_get_item(int,handle,&test_heap_manager) == 5);
    UNIT_TEST_END
    UNIT_TEST_START("serializing and deserializing a heap")
        MEM_heap test_heap,test_heap2;
        MEM_create_heap(MEM_create_heap_template(int,100),&test_heap);
        MEM_add_top(&test_heap,&index);
        MEM_get_item_m(int,&test_heap,index) = 5;
        size_t pos = 0;
        MEM_create_heap(MEM_create_heap_template_not_type(MEM_get_heap_binary_size(&test_heap),1,"mem buff"),&mem_heap);
        MEM_serialize_heap(&test_heap,&pos,&mem_heap);
        pos = 0;
        MEM_deserialize_heap(&test_heap2,&pos,&mem_heap);
        MEM_destroy_heap(&mem_heap);
        ASSERT(MEM_get_item_m(int,&test_heap2,index) == 5);
        MEM_destroy_heap(&test_heap2);
    UNIT_TEST_END
    UNIT_TEST_START("serializing and deserializing an entire heap manager")
        MEM_heap_manager test_heap_manager2;
        add_int();
        size_t pos2 = 0;
        void* ptr1 = malloc(MEM_get_heap_manager_binary_size(&test_heap_manager));
        MEM_create_heap(MEM_create_heap_template_not_type(MEM_get_heap_manager_binary_size(&test_heap_manager),1,"mem buff"),&mem_heap);
        MEM_serialize_heap_manager(&test_heap_manager,&pos2,&mem_heap);
        pos2 = 0;
        MEM_deserialize_heap_manager(&test_heap_manager2,&pos2,&mem_heap);
        MEM_destroy_heap(&mem_heap);
        ASSERT(MEM_get_item_m(int,MEM_get_heap_m(&test_heap_manager2,INT),0) == 5);
    UNIT_TEST_END
END_TESTS
