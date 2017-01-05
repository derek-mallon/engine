#include "tester.h"
#include "sdl_wrapper.h"
WPR_sdl_data data;
MEM_heap_manager mem;
void heap_init(MEM_heap_template* templates,void* data){
    templates[0] = MEM_create_heap_template(WPR_texture_ptr,10);
}
ENVIROMENT_SETUP{
    MEM_create_heap_manager("main_mem",1,heap_init,NULL,&mem);
    WPR_init_sdl_data init_data = {"test_window",1000,500};
    WPR_init_sdl(&data,init_data,&mem.heaps[0]);
}
ENVIROMENT_CLEANUP{
    WPR_cleanup(&data);
}

TESTS
    UNIT_TEST_START("window startup")
        ASSERT(data.window != NULL);
        ASSERT(data.renderer != NULL);
        ASSERT(data.screen_width == 1000);
        ASSERT(data.screen_height == 500);
        ASSERT(data.unit_x == (1000.0/16.0));
        ASSERT(data.unit_y == -(500.0/10.0));
    UNIT_TEST_END

END_TESTS
