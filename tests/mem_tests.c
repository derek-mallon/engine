#include <tester.h>
#include "mem.h"
#include <string.h>

MEM_heap_template test_heap_template;
MEM_heap test_heap;
MEM_heap_manager test_heap_manager;
char buff[50];

enum{
    INT
};
void test_init_fun(MEM_heap_template* templates){
    templates[INT] = MEM_create_heap_template(int,100);
}

void add_int(){
        size_t index;
        MEM_next_free_item(&test_heap_manager.heaps[INT],&index);
        MEM_get_item(int,&test_heap_manager.heaps[INT],index) = 5;
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
        test_heap_manager = MEM_create_heap_manager("test manager",1,test_init_fun);
        ASSERT(test_heap_manager.number_of_heaps == 1);
        ASSERT(test_heap_manager.heaps[INT].size_of_object == sizeof(int));
        ASSERT(test_heap_manager.heaps[INT].capacity == 100);
        ASSERT(test_heap_manager.heaps[INT].ptr != NULL);
        ASSERT(test_heap_manager.heaps[INT].top == 0);
        ASSERT(test_heap_manager.heaps[INT].alive_list != NULL);
        ASSERT(test_heap_manager.heaps[INT].alive_top == 0);
        ASSERT(strcmp(test_heap_manager.heaps[INT].name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("assigning a value")
        test_heap_manager = MEM_create_heap_manager("test manager",1,test_init_fun);
        size_t index;
        if(MEM_next_free_item(&test_heap_manager.heaps[INT],&index) == ERR_BAD){
            ASSERT(0);
        }
        MEM_get_item(int,&test_heap_manager.heaps[INT],index) = 5;
        ASSERT(MEM_get_item(int,&test_heap_manager.heaps[INT],index) == 5);
    UNIT_TEST_END
    UNIT_TEST_START("test adding 4 then deleting a middle one and then add in the middle one's index'")
        test_heap_manager = MEM_create_heap_manager("test manager",1,test_init_fun);
        add_int();
        add_int();
        add_int();
        add_int();
        MEM_free_item(&test_heap_manager.heaps[INT],2);
        size_t index2;
        MEM_next_free_item(&test_heap_manager.heaps[INT],&index2);
        ASSERT(index2 == 2);
    UNIT_TEST_END 
END_TESTS
