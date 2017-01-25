#include "asset.h"
#include "io.h"
#include "dlfcn.h"


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
    /*
    AST_data* data = MEM_get_item_m_p(AST_data,MEM_get_heap(manager,MEM_LOC_AST_DATA),0);
    data->sdl_wrapper_data = MEM_get_item_m_p(WPR_sdl_data,MEM_get_heap(manager,MEM_LOC_WPR_SDL_DATA),0);
    ERR_ASSERT(IO_get_status() == IO_READY,"IO not yet intialized");
    data->texture_paths = MEM_get_heap(manager,MEM_LOC_TEXTURE_PATHS);
    data->component_lib_paths = MEM_get_heap(manager,MEM_LOC_COMPONENT_LIB_PATHS);
    data->audio_paths = MEM_get_heap(manager,MEM_LOC_AUDIO_PATHS);
    MEM_heap* lib_handles = MEM_get_heap(manager,MEM_LOC_COM_LIB_HANDLES);
    //Load all of the textures and put then in the order that they appear in the textures path heap.
    int i;
    for(i=0;i<data->texture_paths->capacity;i++){
        SDL_Surface* surface;
        IO_load_texture_from_image(MEM_get_item_m(FIL_path,data->texture_paths,i),surface);
        WPR_add_texture(data->sdl_wrapper_data,surface,NULL);
    }
    //load all of the libs for the components in order that they appear in the libs path heap.
    for(i=0;i<data->component_lib_paths->capacity;i++){
    AST_lib_open(MEM_get_item_m_p(LIB_HANDLE,lib_handles,i),MEM_get_item_m(UTI_buff_stor,data->component_lib_paths,i).buff);
    }
    */
    //TODO: Audio loading
    //FILE: tracking and live reload debug mode.

}

