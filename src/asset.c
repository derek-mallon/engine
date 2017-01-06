#include "all.h"
#include "io.h"
#include "mem.h"
#include "error.h"

#include "sdl_wrapper.h"
#include "dlfcn.h"

typedef struct AST_data AST_data;


struct AST_data{
    MEM_heap* texture_paths;
    MEM_heap* component_lib_paths;
    MEM_heap* audio_paths;
};

void AST_lib_open(LIB_HANDLE handle,FIL_path path){
    handle = dlopen(path.raw,RTLD_LAZY);
    ERR_ASSERT(handle != NULL,"could not open lib at %s",path.raw)
}

void AST_lib_close(LIB_HANDLE handle){
    dlclose(handle);
}

void AST_init(MEM_heap_manager* manager){
    AST_data* data = &MEM_get_item_m(AST_data,MEM_get_heap(manager,MEM_LOC_AST_DATA),0);
    WPR_sdl_data* sdl_data =  &MEM_get_item_m(WPR_sdl_data,MEM_get_heap(manager,MEM_LOC_WPR_SDL_DATA),0);
    ERR_ASSERT(IO_STATUS == IO_READY,"IO not yet intialized");
    data->texture_paths = MEM_get_heap(manager,MEM_LOC_TEXTURE_PATHS);
    data->component_lib_paths = MEM_get_heap(manager,MEM_LOC_COMPONENT_LIB_PATHS);
    data->audio_paths = MEM_get_heap(manager,MEM_LOC_AUDIO_PATHS);
    MEM_heap* lib_handles = MEM_get_heap(manager,MEM_LOC_COM_LIB_HANDLES);
    //Load all of the textures and put then in the order that they appear in the textures path heap.
    int i;
    for(i=0;i<data->texture_paths->capacity;i++){
        SDL_Surface* surface;
        IO_load_texture_from_image(MEM_get_item_m(FIL_path,data->texture_paths,i),surface);
        WPR_add_texture(sdl_data,surface,NULL);
    }
    //load all of the libs for the components in order that they appear in the libs path heap.
    for(i=0;i<data->component_lib_paths->capacity;i++){
        AST_lib_open(MEM_get_item_m(LIB_HANDLE,lib_handles,i),MEM_get_item_m(FIL_path,data->component_lib_paths,i));
    }
    //TODO: Audio loading
    //FILE: tracking and live reload debug mode.

}

