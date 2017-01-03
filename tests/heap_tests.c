#include <tester.h>
#include "heap.h"
#include <string.h>

heap_template test_heap_template;
heap test_heap;
heap_manager test_heap_manager;
char buff[50];

enum{
    INT
};
void test_init_fun(heap_template* templates){
    templates[INT] = create_heap_template(int,100);
}

ENVIROMENT_SETUP{ //Run before each unit test.
}

ENVIROMENT_CLEANUP{ //Run after each unit test.
    destroy_heap_manager(&test_heap_manager);
}

TESTS
    UNIT_TEST_START("create a heap template")
        test_heap_template = create_heap_template(int,100);
        ASSERT(test_heap_template.size_of_object == sizeof(int));
        ASSERT(test_heap_template.capacity == 100);
        ASSERT(test_heap_template.top == 0);
        ASSERT(strcmp(test_heap_template.name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("create heap_manager with one template")
        test_heap_manager = init_heap_manager("test manager",1,test_init_fun);
        ASSERT(test_heap_manager.number_of_heaps == 1);
        ASSERT(test_heap_manager.heaps[INT].size_of_object == sizeof(int));
        ASSERT(test_heap_manager.heaps[INT].capacity == 100);
        ASSERT(test_heap_manager.heaps[INT].ptr != NULL);
        ASSERT(test_heap_manager.heaps[INT].top == 0);
        ASSERT(strcmp(test_heap_manager.heaps[INT].name,"int") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("assigning a value")
        test_heap_manager = init_heap_manager("test manager",1,test_init_fun);
        get_item(int,(&test_heap_manager.heaps[INT]),0) = 5;
        ASSERT(get_item(int,(&test_heap_manager.heaps[INT]),0) == 5);
    UNIT_TEST_END
END_TESTS
