#include "proj_conf.h"
#include "file_handling.h"
#include "io.h"

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



#define PROJECT_CONF
#ifdef PROJECT_CONF
int main(int argc,char* argv[]){
    PRJ_project_conf conf;
    if(argc >1){
        conf = PRJ_default_conf(argv[1]);
    }else{
        conf = PRJ_default_conf("default");
    }
    PRJ_create_proj(&conf);
    //Save project conf data to file.
    UTI_buff_stor base_data;
    UTI_concat(base_data.buff,2,conf.base_path.buff,"/",PRJ_ROOT_DATA);
    FIL_path root_path = FIL_create_path(base_data.buff,FIL_TYPE_BINARY,FIL_MODE_WRITE | FIL_MODE_OVERWRITE);
    MEM_heap conf_mem;
    MEM_create_heap(MEM_create_heap_template(PRJ_project_conf,1),&conf_mem);
    MEM_get_item_m(PRJ_project_conf,&conf_mem,0) = conf;
    IO_save_heap_binary(&root_path,&conf_mem);
    MEM_destroy_heap(&conf_mem);

    //Search for all of the component mem requirements
    MEM_heap component_templates;
    size_t number_of_components = FIL_get_number_of_dirs_in_dir(conf.component_dir.buff);
    MEM_create_heap(MEM_create_heap_template(MEM_heap_template,number_of_components),&component_templates);
    int i;
    MEM_heap component_sources;
    MEM_create_heap(MEM_create_heap_template(UTI_buff_stor,number_of_components),&component_sources);
    FIL_get_all_dirs(conf.component_dir.buff,&component_sources);
    for(i=0;i<component_templates.capacity;i++){
        size_t number_of_files;
        UTI_buff_stor component_dir_path;
        UTI_concat(component_dir_path.buff,3,conf.component_dir.buff,MEM_get_item_m(UTI_buff_stor,&component_sources,i),"/mem.data");
        ERR_ASSERT(FIL_file_exits(component_dir_path.buff),"this file does no exist %s ",component_dir_path.buff);
        MEM_heap comp_temp_heap;
        FIL_path path = FIL_create_path(component_dir_path.buff,FIL_TYPE_BINARY,FIL_MODE_READ);
        IO_load_heap_binary(&path,&comp_temp_heap);
        MEM_get_item_m(MEM_heap_template,&component_sources,i) = MEM_get_item_m(MEM_heap_template,&comp_temp_heap,0);
        MEM_destroy_heap(&comp_temp_heap);
    }

    UTI_buff_stor std_templates_path;
    UTI_concat(std_templates_path.buff,2,conf.base_path.buff,"/mem.data");
    ERR_ASSERT(FIL_file_exits(std_templates_path.buff),"this file does not exist %s",std_templates_path.buff)
    FIL_path path = FIL_create_path(std_templates_path.buff,FIL_TYPE_BINARY,FIL_MODE_READ);
    MEM_heap std_templates;
    IO_load_heap_binary(&path,&std_templates);

    MEM_destroy_heap(&std_templates);
    MEM_destroy_heap(&component_sources);
}

#endif
