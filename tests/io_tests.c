#include "tester.h"
#include "io.h"
WPR_sdl_data data;
ENVIROMENT_SETUP{
    WPR_init_sdl();
    IO_init();
}
ENVIROMENT_CLEANUP{
    IO_close();
}

TESTS
    UNIT_TEST_START("IO loading texture")
    UNIT_TEST_END
    UNIT_TEST_START("IO saving and loading heap")
        MEM_heap test_heap;
        MEM_create_heap(MEM_create_heap_template(int,10),&test_heap);
        MEM_handle handle = MEM_create_handle_from_heap(&test_heap,0);
        MEM_get_item(int,handle) = 5;
        FIL_path heap_path = FIL_create_path("heap.data",FIL_TYPE_BINARY,FIL_MODE_WRITE | FIL_MODE_OVERWRITE);
        IO_save_heap_binary(&heap_path,&test_heap);
        MEM_destroy_heap(&test_heap);
        heap_path = FIL_create_path("heap.data",FIL_TYPE_BINARY,FIL_MODE_READ);
        IO_load_heap_binary(&heap_path,&test_heap);
        ASSERT(MEM_get_item(int,handle) == 5);
    UNIT_TEST_END
    UNIT_TEST_START("IO saving and loading heap manager")
    UNIT_TEST_END
END_TESTS
