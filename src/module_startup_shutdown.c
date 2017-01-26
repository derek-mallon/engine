#include "sdl_wrapper.h"
#include "io.h"
#include "file_handling.h"
#include "utils.h"
#include "asset.h"
#include "proj_conf.h"
#include "sdl_wrapper.h"
ERR_error startup(MEM_heap_manager* manager,UTI_str path_to_mem){
    FIL_path path = FIL_create_path(path_to_mem,FIL_TYPE_BINARY,FIL_MODE_READ);
    ERR_error result;
    ERR_HANDLE(IO_load_manager_binary(&path,manager));

    WPR_init_sdl_data init_data = {"test_window",1000,500};
    WPR_init_sdl(init_data,manager);

    IO_init();

    AST_init(manager);


    return ERR_GOOD;
}

void shutdown(MEM_heap_manager* manager){
}
