#include "proj_conf.h"
#include "file_handling.h"
#include "io.h"
#include "term.h"
#include "asset.h"
#include "component.h"
#include "sdl_wrapper.h"

ALL_info PRJ_default_conf(UTI_str project_name){
    ALL_info conf;
    UTI_concat(conf.base_path.buff,2,"./",project_name);
    UTI_concat(conf.texture_dir.buff,2,conf.base_path.buff,"/textures");
    UTI_concat(conf.component_dir.buff,2,conf.base_path.buff,"/components");
    UTI_concat(conf.audio_dir.buff,2,conf.base_path.buff,"/audio");
    UTI_concat(conf.bin_dir.buff,2,conf.base_path.buff,"/bin");
    UTI_concat(conf.project_name.buff,1,project_name);
    return conf;
}

void PRJ_create_proj(ALL_info* conf){
   UTI_concat(conf->mem_binary.buff,2,conf->bin_dir.buff,"/main.binary");
   UTI_concat(conf->self.buff,2,conf->bin_dir.buff,"/proj_conf.binary");
   FIL_mkdir(conf->base_path.buff);
   FIL_mkdir(conf->texture_dir.buff);
   FIL_mkdir(conf->component_dir.buff);
   FIL_mkdir(conf->audio_dir.buff);
   FIL_mkdir(conf->bin_dir.buff);
}

void PRJ_create_proj_mem(){

}
/**
 * NOTE: where ever it says runtime it still means the memory is loaded at the beginning of either a scene or the entire game itself.
 * Memory layout is as follows:
 * 0. Project conf
 * 1. System data
 * -- WPR_system
 * -- IO
 * -- Assets
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
typedef struct PRJ_mem_init_data PRJ_mem_init_data;
struct PRJ_mem_init_data{
    ALL_info conf;
    MEM_heap component_mem_templates;
};

void PRJ_mem_init(MEM_heap* templates,void* data_){
    size_t index = MEM_LOC_TOTAL;
    int i;
    int j;
    PRJ_mem_init_data* data = data_;
    MEM_get_item_m(MEM_heap_template,templates,MEM_LOC_INFO) = MEM_create_heap_template(ALL_info,1);
    MEM_get_item_m(MEM_heap_template,templates,MEM_LOC_WPR_SDL_DATA) = MEM_create_heap_template(WPR_sdl_data,1);
    MEM_get_item_m(MEM_heap_template,templates,MEM_LOC_AST_DATA) = MEM_create_heap_template(AST_data,1);
    MEM_get_item_m(MEM_heap_template,templates,MEM_LOC_AUDIO_DATA) =  MEM_create_heap_template(AST_audio_data,data->conf.number_of_audio_files);
    MEM_get_item_m(MEM_heap_template,templates,MEM_LOC_TEXTURE_DATA) =  MEM_create_heap_template(AST_texture_data,data->conf.number_of_textures);
    MEM_get_item_m(MEM_heap_template,templates,MEM_LOC_ENTITIES) = MEM_create_heap_template_not_type(sizeof(MEM_handle)*data->conf.number_of_components,data->conf.entity_capacity,"entities");
    
    for(i=0;i<data->conf.number_of_components;i++){
        MEM_get_item_m(MEM_heap_template,templates,index) = MEM_create_heap_template(COM_component,1);
        index++;
        MEM_get_item_m(MEM_heap_template,templates,index) = MEM_get_item_m_p(COM_component_mem_template,&data->component_mem_templates,i)->template;
        index++;

        for(j=0;j<MEM_get_item_m_p(COM_component_mem_template,&data->component_mem_templates,i)->number_of_messages;j++){
            MEM_get_item_m(MEM_heap_template,templates,index) = MEM_create_heap_template(LIB_FUNC,1);
            index++;
        }
    }

    ERR_ASSERT(index==templates->capacity,"not enough mem set %lu %lu",index,templates->capacity);
    MEM_destroy_heap(&data->component_mem_templates);
}
void PRJ_create_proj_binary(ALL_info conf){

    PRJ_mem_init_data data;

    MEM_heap component_lib_paths;
    MEM_heap_manager manager;
    UTI_buff_stor path_data;


    size_t top = MEM_LOC_TOTAL;
    int i;

    data.conf = conf;
    data.conf.number_of_components = FIL_get_number_of_files_in_dir(conf.component_dir.buff);
    data.conf.number_of_textures = FIL_get_number_of_files_in_dir(conf.texture_dir.buff);
    data.conf.number_of_audio_files = FIL_get_number_of_files_in_dir(conf.audio_dir.buff);

    top += 2*data.conf.number_of_components;

    //Gets destroyed in this function
    MEM_create_heap(MEM_create_heap_template(UTI_buff_stor,data.conf.number_of_components),&component_lib_paths);
    //Gets destroyed in the mem init function
    MEM_create_heap(MEM_create_heap_template(COM_component_mem_template,data.conf.number_of_components),&data.component_mem_templates);

    FIL_get_all_files(conf.component_dir.buff,&component_lib_paths);

    for(i=0;i<data.conf.number_of_components;i++){
    
        UTI_concat(path_data.buff,3,conf.component_dir.buff,"/",MEM_get_item_m(UTI_buff_stor,&component_lib_paths,i).buff);
        LIB_HANDLE handle = AST_lib_open(path_data.buff);
        
        size_t (*get_size)(void) = AST_get_func(handle,"get_size");
        size_t (*get_capacity)(void) = AST_get_func(handle,"get_capacity");
        size_t (*get_number_of_messages)(void) = AST_get_func(handle,"get_number_of_messages");

        MEM_get_item_m(COM_component_mem_template,&data.component_mem_templates,i).template = MEM_create_heap_template_not_type(get_size(),get_capacity(),MEM_get_item_m(UTI_buff_stor,&component_lib_paths,i).buff);
        MEM_get_item_m(COM_component_mem_template,&data.component_mem_templates,i).number_of_messages = get_number_of_messages();
        top += get_number_of_messages();

        AST_lib_close(handle);
    }


    
    //Gets destroyed when saved
    MEM_create_heap_manager("main_mem",top,PRJ_mem_init,&data,&manager);
    
    
    FIL_path binary_path = FIL_create_path(conf.mem_binary.buff,FIL_TYPE_BINARY,FIL_MODE_WRITE | FIL_MODE_OVERWRITE);
    MEM_get_item_m(ALL_info,MEM_get_heap(&manager,MEM_LOC_INFO),0) = data.conf;
    IO_save_manager_binary(&binary_path,&manager);
    MEM_destroy_heap(&component_lib_paths);
}

ERR_error PRJ_load_proj_conf(UTI_str path,ALL_info* conf){
    FIL_path path_ = FIL_create_path(path,FIL_TYPE_BINARY,FIL_MODE_READ);
    MEM_heap mem;
    ERR_error result;
    if((result = IO_load_heap_binary(&path_,&mem)) != ERR_GOOD){
        return result;
    }
    *conf =  MEM_get_item_m(ALL_info,&mem,0);
    MEM_destroy_heap(&mem);
    return ERR_GOOD;
}

