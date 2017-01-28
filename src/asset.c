#include "asset.h"
#include "dlfcn.h"
#include "file_handling.h"
#include <string.h>



static AST_status AST_STATUS = AST_CLOSED;

LIB_HANDLE AST_lib_open(const char* str){
    LIB_HANDLE handle = dlopen(str,RTLD_LAZY);
    ERR_ASSERT(handle != NULL,"could not open lib at %s",str);
    return handle;
}

void AST_lib_close(LIB_HANDLE handle){
    dlclose(handle);
}


LIB_FUNC AST_get_func(LIB_HANDLE handle,const char* str){
    return dlsym(handle,str);
}

typedef struct walk_struct_data{
    AST_data* data;
}walk_struct_data;

static void walk_texture_dir(const char* texture_name,void* data_,int count){
    AST_data* data = ((walk_struct_data*)data_)->data;
    char* buff = malloc(strlen("textures/") + strlen(texture_name)+1);
    UTI_concat(buff,2,"textures/",texture_name);
    AST_load_texture(data,MEM_get_item_m_p(AST_texture_data,data->texture_data,count),buff);
}

static void walk_audio_dir(const char* audio_name,void* data_,int count){
    AST_data* data = ((walk_struct_data*)data_)->data;
    char* buff = malloc(strlen("audio/") + strlen(audio_name)+1);
    UTI_concat(buff,2,"audio/",audio_name);
    //load audio file
    free(buff);
}
static void find_all_and_load_textures(AST_data* data){
    walk_struct_data data_ =  {data};
    FIL_walk_over_all_files_in_dir("textures",walk_texture_dir,(void*)&data_);
}

static void find_all_and_load_audio_files(AST_data* data){
    walk_struct_data data_ =  {data};
    FIL_walk_over_all_files_in_dir("audio",walk_texture_dir,(void*)&data_);
}


ERR_error AST_init(MEM_heap_manager* manager){
    ERR_ASSERT(WPR_get_status() == WPR_READY,"sdl_wrapper not ready");

    MEM_handle handle = MEM_create_handle(MEM_LOC_AST_DATA,0);
    AST_data* data = MEM_get_item_p(AST_data,handle,manager);
    data->audio_data = MEM_get_heap_m(manager,MEM_LOC_AUDIO_DATA);
    data->texture_data = MEM_get_heap_m(manager,MEM_LOC_TEXTURE_DATA);
    data->sdl_data = MEM_get_item_m_p(WPR_sdl_data,MEM_get_heap_m(manager,MEM_LOC_WPR_SDL_DATA),0);
    find_all_and_load_textures(data);

    AST_STATUS = AST_READY;
    return ERR_BAD;

}


ERR_error AST_load_texture(AST_data* data,AST_texture_data* texture,const char* path){
    ERR_error result;
    ERR_HANDLE(WPR_load_texture_from_image(path,data->sdl_data,&texture->texture_buff));
    return ERR_GOOD;
}


void AST_shudown(MEM_heap_manager* manager){
    MEM_handle handle = MEM_create_handle(MEM_LOC_AST_DATA,0);
    AST_data* data = MEM_get_item_p(AST_data,handle,manager);
}

AST_status AST_get_status(){
    return AST_STATUS;
}

