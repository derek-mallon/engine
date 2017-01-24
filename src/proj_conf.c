#include "proj_conf.h"
#include "file_handling.h"
#include "io.h"
#include "term.h"
#include "asset.h"

PRJ_project_conf PRJ_default_conf(UTI_str project_name){
    PRJ_project_conf conf;
    UTI_concat(conf.base_path.buff,2,"./",project_name);
    UTI_concat(conf.texture_dir.buff,2,conf.base_path.buff,"/textures");
    UTI_concat(conf.component_dir.buff,2,conf.base_path.buff,"/components");
    UTI_concat(conf.bin_dir.buff,2,conf.base_path.buff,"/bin");
    return conf;
}

void PRJ_create_proj(PRJ_project_conf* conf){
   FIL_mkdir(conf->base_path.buff);
   FIL_mkdir(conf->texture_dir.buff);
   FIL_mkdir(conf->component_dir.buff);
   FIL_mkdir(conf->bin_dir.buff);
}

void PRJ_create_proj_mem(){

}

/**
 * NOTE: where ever it says runtime it still means the memory is loaded at the beginning of either a scene or the entire game itself.
 * Memory layout is as follows:
 * 1. System data
 * -- WPR_system
 * -- IO
 * -- Asset 
 * -- 
 * 2. Asset Data ----> all Editable at runtime but must be loaded and cleaned up at run time.
 * -- Wav files {type: WPR_audio_buff,str path} 
 * -- Textures  {type: WPR_texture_ptr , str path} loaded and cleanup at runtime.
 * -- Shaders ---> get compiled into the correct type
 * -- Prefab files ---> gets compiled down into the correct type (depends of the number of componets)
 * 3. Component data   
 * -- Component 1
 * --- Component Standard data (init update destroy, size, and message number information) {type: COM_component} --------> Editable during run time
 * --- Component Custom data[] (array for all of the components) {type: custom}
 * --- Message 1 --------> Editable during run time {type void * (gets cast at runtime to the correct function pointer also get cleaned up at run time)}
 * --- Message 2 --------> Editable during run time
 * --- ......
 * --- Message n
 * -- Component 2
 * --- Component Standard data (init update destroy, size, and message number information) --------> Editable during run time
 * --- Component Custom data[] (array for all of the components)
 * --- Messages ...
 * -- Components ..   
 * 4. Entity data
 *
 * This is all allocated and then saved to disk. When the game starts the memory manager is deserialized from a binary file.
 *
 * How components memory works.
 * 1. Component is coded up as a standalone shared library with "get_size", "get_capacity", "get_number_of_messages", "get_messages", "init", "update", "destroy", and message functions.
 * 2. During project creation the shared libraries are linked via dfcon (POSIX) and SharedLibary (win32). Then the project creator retieves the needed information about the struct size and or the number of messages .
 * 3. During run time the linking is only needed for the procedures.
 *
 * How the memory is build
 * 1. First make all of the needed mem_templates for the system 
 * 2. Then make all of the needed mem_templates for the assets (currently only textures) this are loaded at run time by SDL_image but their pointers are saved before hand.
 * 3. Then all of the components located in the components dir are searched for.
 * 4. Loop over the components and create COM_component_mem_template for each one.
 * 5. Loop over the COM_component_mem_templates and  create the correct mem_heaps for each one building out a mem_manager
 * 6. Using the known number of components create the entity arrays.
 * 6. Save the mem_manager to disk.
 */
void PRJ_create_proj_binary(PRJ_project_conf* conf){
    UTI_buff_stor base_data;
    UTI_concat(base_data.buff,2,conf->base_path.buff,"/",PRJ_ROOT_DATA);
    FIL_path root_path = FIL_create_path(base_data.buff,FIL_TYPE_BINARY,FIL_MODE_WRITE | FIL_MODE_OVERWRITE);
    MEM_heap conf_mem;
    MEM_create_heap(MEM_create_heap_template(PRJ_project_conf,1),&conf_mem);
    MEM_get_item_m(PRJ_project_conf,&conf_mem,0) = *conf;
    IO_save_heap_binary(&root_path,&conf_mem);
    MEM_destroy_heap(&conf_mem);

    //Search for all of the component mem requirements
    MEM_heap component_templates;
    size_t number_of_components = FIL_get_number_of_files_in_dir(conf->component_dir.buff);
    int i;
    MEM_heap component_libs;
    MEM_create_heap(MEM_create_heap_template(UTI_buff_stor,number_of_components),&component_libs);
    FIL_get_all_files(conf->component_dir.buff,&component_libs);
    for(i=0;i<number_of_components;i++){
       LIB_HANDLE handle =AST_lib_open(MEM_get_item_m(UTI_buff_stor,&component_libs,i).buff);

       size_t (*get_size)(void) = AST_get_func(handle,"get_size");
       size_t comp_size = get_size();

       size_t (*get_capacity)(void) = AST_get_func(handle,"get_capacity");
       size_t comp_capacity = get_capacity();

       size_t (*get_number_of_messages)(void) = AST_get_func(handle,"get_number_of_messages");
       number_of_components += get_number_of_messages();


       AST_lib_close(handle);

    }

    UTI_buff_stor std_templates_path;
    UTI_concat(std_templates_path.buff,2,conf->base_path.buff,"/mem.data");
    ERR_ASSERT(FIL_file_exits(std_templates_path.buff),"this file does not exist %s",std_templates_path.buff);
    FIL_path path = FIL_create_path(std_templates_path.buff,FIL_TYPE_BINARY,FIL_MODE_READ);
    MEM_heap std_templates;
    IO_load_heap_binary(&path,&std_templates);


    MEM_destroy_heap(&std_templates);
    MEM_destroy_heap(&component_libs);
}

