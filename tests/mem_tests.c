#include <tester.h>
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
void test_init_fun(MEM_heap_template* templates,void* data){
    templates[INT] = MEM_create_heap_template(int,100);
}

void add_int(){
        size_t index;
        MEM_add_top(&test_heap_manager.heaps[INT],&index);
        MEM_handle handle = MEM_create_handle_from_manager(&test_heap_manager,INT,index);
        MEM_get_item(int,handle) = 5;
}

ENVIROMENT_SETUP{ //Run before each unit test.
}

ENVIROMENT_CLEANUP{ //Run after each unit test.
    MEM_destroy_heap_manager(&test_heap_manager);
}

TESTS
    UNIT_TEST_START("create a heap template")
        test_heap_template = MEM_create_heap_template(int,100);
        ASSERT(test_heap_template.size_of_object == sizeof(int));
        ASSERT(test_heap_template.capacity == 100);
        ASSERT(strcmp(test_heap_template.name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("create heap_manager with one template")
        MEM_create_heap_manager("test manager",1,test_init_fun,NULL,&test_heap_manager);
        ASSERT(test_heap_manager.number_of_heaps == 1);
        ASSERT(test_heap_manager.heaps[INT].size_of_object == sizeof(int));
        ASSERT(test_heap_manager.heaps[INT].capacity == 100);
        ASSERT(test_heap_manager.heaps[INT].ptr != NULL);
        ASSERT(test_heap_manager.heaps[INT].top == 0);
        ASSERT(strcmp(test_heap_manager.heaps[INT].name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("assigning a value")
        MEM_create_heap_manager("test manager",1,test_init_fun,NULL,&test_heap_manager);
        size_t index;
        handle = MEM_create_handle_from_manager(&test_heap_manager,INT,index);
        MEM_get_item(int,handle) = 5;
        ASSERT(MEM_get_item(int,handle) == 5);
    UNIT_TEST_END
    UNIT_TEST_START("serializing and deserializing a heap")
        MEM_heap test_heap,test_heap2;
        MEM_create_heap(MEM_create_heap_template(int,100),&test_heap);
        MEM_add_top(&test_heap,&index);
        handle = MEM_create_handle_from_heap(&test_heap,index);
        MEM_get_item(int,handle) = 5;
        size_t pos = 0;
        MEM_create_heap(MEM_create_heap_template_not_type(MEM_get_heap_binary_size(&test_heap),1,"mem buff"),&mem_heap);
        handle = MEM_create_handle_from_heap(&mem_heap,0);
        MEM_serialize_heap(&test_heap,&pos,handle);
        pos = 0;
        MEM_deserialize_heap(&test_heap2,&pos,handle);
        MEM_destroy_heap(&mem_heap);
        handle = MEM_create_handle_from_heap(&test_heap2,index);
        ASSERT(MEM_get_item(int,handle) == 5);
    UNIT_TEST_END
    UNIT_TEST_START("serializing and deserializing an entire heap manager")
        MEM_heap_manager test_heap_manager2;
        MEM_create_heap_manager("test manager",1,test_init_fun,NULL,&test_heap_manager);
        
        add_int();
        size_t pos2 = 0;
        void* ptr1 = malloc(MEM_get_heap_manager_binary_size(&test_heap_manager));
        MEM_create_heap(MEM_create_heap_template_not_type(MEM_get_heap_manager_binary_size(&test_heap_manager),1,"mem buff"),&mem_heap);
        handle = MEM_create_handle_from_heap(&mem_heap,0);
        MEM_serialize_heap_manager(&test_heap_manager,&pos2,handle);
        pos2 = 0;
        MEM_deserialize_heap_manager(&test_heap_manager2,&pos2,handle);
        MEM_destroy_heap(&mem_heap);
        handle = MEM_create_handle_from_manager(&test_heap_manager2,INT,0);
        ASSERT(MEM_get_item(int,handle) == 5);
    UNIT_TEST_END
END_TESTS
