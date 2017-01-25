#include "sdl_wrapper.h"
#include "io.h"
#include "file_handling.h"
#include "utils.h"
#include "asset.h"
#include "proj_conf.h"
#include "sdl_wrapper.h"
ERR_error startup(MEM_heap_manager* manager,UTI_str path_to_conf){
    ALL_info conf;
    PRJ_load_proj_conf(path_to_conf,&conf);
    FIL_path path = FIL_create_path(conf.mem_binary.buff,FIL_TYPE_BINARY,FIL_MODE_READ);
    ERR_error result;
    if((result = IO_load_manager_binary(&path,manager)) != ERR_GOOD){
        return result;
    }
    MEM_get_item_m(ALL_info,MEM_get_heap(manager,MEM_LOC_INFO),0) = conf;

    WPR_init_sdl_data init_data = {"test_window",1000,500};
    WPR_init_sdl(init_data,manager);

    IO_init();

    AST_init(manager);


    return ERR_GOOD;
}

void shutdown(MEM_heap_manager* manager){
}
