#include "sdl_wrapper.h"
#include "file_handling.h"
#include "utils.h"
#include "asset.h"
#include "proj_conf.h"
#include "sdl_wrapper.h"
#include "serialize_output.h"

void create_memory(MEM_heap* heap,void* data){
    
    /*
    size_t index = MEM_LOC_TOTAL;
    int i;
    int j;
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
        MEM_get_item_m(MEM_heap_template,templates,index) = MEM_create_heap_template(LIB_FUNC,MEM_get_item_m_p(COM_component_mem_template,&data->component_mem_templates,i)->number_of_messages);
        index++;
        */
}

ERR_error startup(MEM_heap_manager* manager,const char* path_to_mem){

    int i;
    WPR_init_sdl_data init_data = {"test_window",1000,500};
    WPR_init_sdl(init_data,manager);


    AST_init(manager);


    return ERR_GOOD;
}

void shutdown(MEM_heap_manager* manager){
    AST_shudown(manager);
}
