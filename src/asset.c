#include "asset.h"
#include "io.h"
#include "dlfcn.h"


static AST_status AST_STATUS = AST_CLOSED;

LIB_HANDLE AST_lib_open(UTI_str str){
    LIB_HANDLE handle = dlopen(str,RTLD_LAZY);
    ERR_ASSERT(handle != NULL,"could not open lib at %s",str);
    return handle;
}

void AST_lib_close(LIB_HANDLE handle){
    dlclose(handle);
}


LIB_FUNC AST_get_func(LIB_HANDLE handle,UTI_str str){
    return dlsym(handle,str);
}

typedef struct walk_struct_data{
    AST_data* data;
    size_t index;
}walk_struct_data;

static void walk_texture_dir(UTI_str texture_name,void* data_){
    AST_data* data = ((walk_struct_data*)data_)->data;
    size_t index = ((walk_struct_data*)data_)->index;
    ((walk_struct_data*)data_)->index++;
    UTI_concat(MEM_get_item_m(AST_texture_data,data->texture_data,index).path.buff,3,data->info->texture_dir,"/",texture_name);
}

static void walk_audio_dir(UTI_str audio_name,void* data_){
    AST_data* data = ((walk_struct_data*)data_)->data;
    size_t index = ((walk_struct_data*)data_)->index;
    ((walk_struct_data*)data_)->index++;
    UTI_concat(MEM_get_item_m(AST_audio_data,data->audio_data,index).path.buff,3,data->info->audio_dir,"/",audio_name);
}
static void find_all_textures(AST_data* data){
    walk_struct_data data_ =  {data,0};
    FIL_walk_over_all_files_in_dir(data->info->texture_dir.buff,walk_texture_dir,(void*)&data_);
}

static void find_all_audio_files(AST_data* data){
    walk_struct_data data_ =  {data,0};
    FIL_walk_over_all_files_in_dir(data->info->audio_dir.buff,walk_texture_dir,(void*)&data_);
}

static ERR_error AST_load_(AST_data* data){
    int i;
    AST_texture_data* texture;
    for(i=0;i<data->info->number_of_textures;i++){
        texture = MEM_get_item_m_p(AST_texture_data,data->texture_data,i);
        SDL_Surface* surface;
        ERR_error result;
        ERR_HANDLE(IO_load_texture_from_image(texture->path.buff,surface));
        ERR_HANDLE(WPR_turn_surface_into_texture(data->sdl_data,surface,&texture->texture_buff));
    }
    for(i=0;i<data->info->number_of_audio_files;i++){
        //load audio files.
    }
    return ERR_GOOD;
}

void AST_init(MEM_heap_manager* manager){
    ERR_ASSERT(WPR_get_status() == WPR_READY,"sdl_wrapper not ready");
    ERR_ASSERT(IO_get_status() == IO_READY,"io not ready");

    MEM_handle handle = MEM_create_handle(MEM_LOC_AST_DATA,0);
    AST_data* data = MEM_get_item_p(AST_data,handle,manager);
    data->audio_data = MEM_get_heap_m(manager,MEM_LOC_AUDIO_DATA);
    data->texture_data = MEM_get_heap_m(manager,MEM_LOC_TEXTURE_DATA);
    //Cache the sdl_data for faster use of sdl_context to load textures
    data->sdl_data = MEM_get_item_m_p(WPR_sdl_data,MEM_get_heap_m(manager,MEM_LOC_WPR_SDL_DATA),0);
    data->info =  MEM_get_item_m_p(ALL_info,MEM_get_heap_m(manager,MEM_LOC_INFO),0);

    find_all_textures(data);
    find_all_audio_files(data);
    AST_load_(data);
    AST_STATUS = AST_READY;

}


ERR_error AST_load_texture(AST_data* data,AST_texture_data* texture){
    SDL_Surface* surface;
    ERR_error result;
    ERR_HANDLE(IO_load_texture_from_image(texture->path.buff,surface));
    ERR_HANDLE(WPR_turn_surface_into_texture(data->sdl_data,surface,&texture->texture_buff));
    return ERR_GOOD;
}


void AST_shudown(MEM_heap_manager* manager);

AST_status AST_get_status(){
    return AST_STATUS;
}

