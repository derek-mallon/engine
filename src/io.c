#include "io.h"
#include "error.h"
#include <SDL2/SDL_image.h>
#include "sdl_wrapper.h"

static IO_status IO_STATUS = IO_CLOSED;
void IO_init(){
    ERR_ASSERT(WPR_get_status() == WPR_READY,"SDL wrapper not inited");
    int result = IMG_Init(IMG_INIT_PNG);
    ERR_ASSERT((result == IMG_INIT_PNG),"Couldn't init SDL_image: %s",IMG_GetError());
    IO_STATUS = IO_READY;
}


ERR_error IO_load_texture_from_image(UTI_str path,SDL_Surface* surface){
    surface= IMG_Load(path);
    if(!surface){
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't load the image at %s: %s",path,SDL_GetError());
        return ERR_BAD;
    }
    return ERR_GOOD;
}

void IO_close(){
    IMG_Quit();
    IO_STATUS = IO_CLOSED;
}
ERR_error IO_load_manager_binary(FIL_path* path,MEM_heap_manager* manager){
    ERR_ASSERT((path->mode & FIL_MODE_READ) && !(path->mode & FIL_MODE_OVERWRITE) && !(path->mode & FIL_MODE_WRITE) && ((path->mode & FIL_MODE_BINARY) || path->type == FIL_TYPE_BINARY),"incorrect file mode %s",path->ops);

    size_t pos = 0;
    MEM_heap mem_heap;

    FIL_file_open(path);
    ERR_error result;
    ERR_HANDLE(MEM_create_heap(MEM_create_heap_template_not_type(FIL_file_size_binary(path),1,"file mem"),&mem_heap));
    ERR_HANDLE(FIL_read_binary(path,&mem_heap));

    ERR_HANDLE(MEM_deserialize_heap_manager(manager,&pos,&mem_heap));
    MEM_destroy_heap(&mem_heap);
    FIL_file_close(path);
    return ERR_GOOD;
}
ERR_error IO_save_manager_binary(FIL_path* path,MEM_heap_manager* manager){
    ERR_ASSERT(!(path->mode & FIL_MODE_READ) && (path->mode & FIL_MODE_OVERWRITE) && (path->mode & FIL_MODE_WRITE) && ((path->mode & FIL_MODE_BINARY) || path->type == FIL_TYPE_BINARY),"incorrect file mode %s",path->ops);

    size_t pos = 0;
    MEM_heap mem_heap;
    ERR_error result;

    ERR_HANDLE(FIL_file_open(path));

    ERR_HANDLE(MEM_create_heap(MEM_create_heap_template_not_type(MEM_get_heap_manager_binary_size(manager),1,"file mem"),&mem_heap))

    MEM_serialize_heap_manager(manager,&pos,&mem_heap);
    ERR_HANDLE(FIL_write_binary(path,&mem_heap));
    FIL_file_close(path);
    return ERR_GOOD;
}

ERR_error IO_load_heap_binary(FIL_path* path,MEM_heap* heap){
    ERR_ASSERT((path->mode & FIL_MODE_READ) && !(path->mode & FIL_MODE_OVERWRITE) && !(path->mode & FIL_MODE_WRITE) && ((path->mode & FIL_MODE_BINARY) || path->type == FIL_TYPE_BINARY),"incorrect file mode %s",path->ops);

    MEM_heap mem_heap;
    size_t pos = 0;

    ERR_error result;
    ERR_HANDLE(FIL_file_open(path));
    ERR_HANDLE(MEM_create_heap(MEM_create_heap_template_not_type(FIL_file_size_binary(path),1,"file mem"),&mem_heap));
    ERR_HANDLE(FIL_read_binary(path,&mem_heap));
    ERR_HANDLE(MEM_deserialize_heap(heap,&pos,&mem_heap));
    ERR_HANDLE(MEM_destroy_heap(&mem_heap));
    FIL_file_close(path);
    return ERR_GOOD;
}
ERR_error IO_save_heap_binary(FIL_path* path,MEM_heap* heap){
    ERR_ASSERT(!(path->mode & FIL_MODE_READ) && (path->mode & FIL_MODE_OVERWRITE) && (path->mode & FIL_MODE_WRITE) && ((path->mode & FIL_MODE_BINARY) || path->type == FIL_TYPE_BINARY),"incorrect file mode %s",path->ops);
    MEM_heap mem_heap;

    size_t pos = 0;
    FIL_file_open(path);

    ERR_error result;
    ERR_HANDLE(MEM_create_heap(MEM_create_heap_template_not_type(MEM_get_heap_binary_size(heap),1,"file mem"),&mem_heap));
    MEM_serialize_heap(heap,&pos,&mem_heap);
    ERR_HANDLE(FIL_write_binary(path,&mem_heap));
    FIL_file_close(path);
    ERR_HANDLE(MEM_destroy_heap(&mem_heap));
    return ERR_GOOD;
}

IO_status IO_get_status(){
    return IO_STATUS;
}

