#include <tester.h>
#include "file_handling.h"
#include <string.h>
#include "mem.h"
FIL_path path;
ENVIROMENT_SETUP{
}
ENVIROMENT_CLEANUP{
}

void heap_init(MEM_heap_template* templates,void* data){
    templates[0] = MEM_create_heap_template(UTI_buff_stor,10);
}
TESTS
    UNIT_TEST_START("create path")
        path = FIL_create_path("test.txt",FIL_TYPE_TEXT,FIL_MODE_WRITE);
        ASSERT(strcmp(path.raw,"test.txt") == 0);
        ASSERT(strcmp(path.extension,".txt") == 0);
        ASSERT(strcmp(path.ops,"a") == 0);
        path = FIL_create_path("test.txt",FIL_TYPE_TEXT,(FIL_MODE_WRITE | FIL_MODE_READ));
        ASSERT(strcmp(path.ops,"r+") == 0);
        path = FIL_create_path("test.txt",FIL_TYPE_TEXT,(FIL_MODE_WRITE | FIL_MODE_READ | FIL_MODE_BINARY));
        //ASSERT(strcmp(path.ops,"r+b") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("file exists")
        ASSERT(FIL_file_exits("bin/file_handling_tests"));
    UNIT_TEST_END
    UNIT_TEST_START("Open a new file")
        path = FIL_create_path("test.tmp",FIL_TYPE_TEXT,FIL_MODE_WRITE);
        FIL_file_open(&path);
        ASSERT(FIL_file_exits("test.tmp"));
        FIL_file_close(&path);
        FIL_remove_file(path.raw);
    UNIT_TEST_END
    UNIT_TEST_START("find all files in dir")
        path = FIL_create_path("test.tmp",FIL_TYPE_TEXT,FIL_MODE_WRITE);
        FIL_file_open(&path);
        MEM_heap_manager manager;
        MEM_create_heap_manager("main",1,heap_init,NULL,&manager);
        FIL_get_all_files(".",&manager.heaps[0]);
        int i;
        uint8_t found = 0;
        for(i=0;i<manager.heaps[0].top;i++){
            if(strcmp(MEM_get_item(UTI_buff_stor,&manager.heaps[0],i).buff,"test.tmp") == 0){
                found = 1;
            }
        }
        ASSERT(found);
    UNIT_TEST_END
END_TESTS

