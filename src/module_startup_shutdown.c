#include "sdl_wrapper.h"
#include "io.h"
#include "file_handling.h"
#include "utils.h"
#include "asset.h"
#include "proj_conf.h"
ERR_error startup(MEM_heap_manager* manager,UTI_str path_to_conf){
    PRJ_project_conf conf;
    PRJ_load_proj_conf(path_to_conf,&conf);
    FIL_path path = FIL_create_path(conf.mem_binary.buff,FIL_TYPE_BINARY,FIL_MODE_READ);
    ERR_error result;
    if((result = IO_load_manager_binary(&path,manager)) != ERR_GOOD){
        return result;
    }
    MEM_get_item_m(PRJ_project_conf,MEM_get_heap(manager,MEM_LOC_PROJ_CONF),0) = conf;
     
    return ERR_GOOD;
}

void shutdown(MEM_heap_manager* manager){
}
