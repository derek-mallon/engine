#include <tester.h>
#include "file_handling.h"
#include <string.h>
FIL_path path;
ENVIROMENT_SETUP{
}
ENVIROMENT_CLEANUP{
}
TESTS
    printf("%d \n" , FIL_MODE_WRITE &(FIL_MODE_READ | FIL_MODE_WRITE));
    UNIT_TEST_START("create path")
        path = FIL_create_path("test.txt",FIL_TYPE_TEXT,FIL_MODE_WRITE);
        ASSERT(strcmp(path.raw,"test.txt") == 0);
        ASSERT(strcmp(path.extension,".txt") == 0);
        ASSERT(strcmp(path.ops,"a") == 0);
        printf("%s",path.ops);
        path = FIL_create_path("test.txt",FIL_TYPE_TEXT,(FIL_MODE_WRITE | FIL_MODE_READ));
        ASSERT(strcmp(path.ops,"r+") == 0);
        path = FIL_create_path("test.txt",FIL_TYPE_TEXT,(FIL_MODE_WRITE | FIL_MODE_READ | FIL_MODE_BINARY));
        //ASSERT(strcmp(path.ops,"r+b") == 0);
    UNIT_TEST_END
    UNIT_TEST_START("file exists")
        ASSERT(FIL_file_exits("file_handling_tests"));
    UNIT_TEST_END
    UNIT_TEST_START("Open a new file")
        path = FIL_create_path("test.tmp",FIL_TYPE_TEXT,FIL_MODE_WRITE);

        ASSERT(FIL_file_exits("file_handling_tests"));
    UNIT_TEST_END
END_TESTS

