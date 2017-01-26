#include "tester.h"
#include "io.h"
#include "sdl_wrapper.h"
#include "mem.h"
/*
WPR_sdl_data data;
MEM_heap textures;

ENVIROMENT_SETUP{
    WPR_init_sdl_data init_data = {"test_window",1000,500};
    MEM_create_heap(MEM_create_heap_template(WPR_texture_ptr,0),&textures);
    WPR_init_sdl(init_data,NULL);
    IO_init();
}
ENVIROMENT_CLEANUP{
    IO_close();
    WPR_cleanup(&data);
    MEM_destroy_heap(&textures);
}
*/

TESTS
/*
    UNIT_TEST_START("IO loading texture")
    UNIT_TEST_END
    UNIT_TEST_START("IO saving and loading heap")
        MEM_heap test_heap;
        MEM_create_heap(MEM_create_heap_template(int,10),&test_heap);
        printf("CAP:::%lu",test_heap.capacity);
        MEM_handle handle = MEM_create_handle_from_heap(&test_heap,0);
        MEM_get_item(int,handle) = 5;
        ASSERT(MEM_get_item(int,handle) == 5);
        FIL_path heap_path = FIL_create_path("heap.data",FIL_TYPE_BINARY,FIL_MODE_WRITE | FIL_MODE_OVERWRITE);
        IO_save_heap_binary(&heap_path,&test_heap);
        MEM_destroy_heap(&test_heap);
        heap_path = FIL_create_path("heap.data",FIL_TYPE_BINARY,FIL_MODE_READ);
        IO_load_heap_binary(&heap_path,&test_heap);
        handle = MEM_create_handle_from_heap(&test_heap,0);
        ASSERT(MEM_get_item(int,handle) == 5);
    UNIT_TEST_END
*/
END_TESTS
