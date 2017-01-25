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

void AST_init(MEM_heap_manager* manager){
    ERR_ASSERT(WPR_get_status() == WPR_READY,"sdl_wrapper not ready");
    ERR_ASSERT(IO_get_status() == IO_READY,"io not ready");

    MEM_handle handle = MEM_create_handle_from_manager(manager,MEM_LOC_AST_DATA,0);
    AST_data* data = MEM_get_item_p(AST_data,handle);
    data->audio_data = MEM_get_heap(manager,MEM_LOC_AUDIO_DATA);
    data->texture_data = MEM_get_heap(manager,MEM_LOC_TEXTURE_DATA);
    //Cache the sdl_data for faster use of sdl_context to load textures
    data->sdl_data = MEM_get_item_m_p(WPR_sdl_data,MEM_get_heap(manager,MEM_LOC_WPR_SDL_DATA),0);
    AST_load(manager);

    AST_STATUS = AST_READY;
}
/*
ERR_error AST_find_textures(AST_data* data){
    MEM_heap texture_paths;

}
*/

ERR_error AST_load_texture(AST_data* data,AST_texture_data* texture){
    SDL_Surface* surface;
    ERR_error result;
    if((result = IO_load_texture_from_image(texture->path.buff,surface)) != ERR_GOOD){
        return result;
    }
    if((result = WPR_turn_surface_into_texture(data->sdl_data,surface,&texture->texture_buff)) != ERR_GOOD){
        return result;
    }
    return ERR_GOOD;
}

void AST_load(MEM_heap_manager* manager){

}

void AST_shudown(MEM_heap_manager* manager);

AST_status AST_get_status(){
    return AST_STATUS;
}

