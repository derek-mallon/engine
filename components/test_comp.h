#ifndef TEST_COMP
#define TEST_COMP
#include "mem.h"

typedef struct COMPONENT_test_comp COMPONENT_test_comp;

struct COMPONENT_test_comp{
    float x;
    float y;
};

void MESSAGE_update(MEM_handle self,MEM_handle messanger,MEM_heap_manager* manager);
#endif

