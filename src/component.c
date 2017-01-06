#include "all.h"
#include "mem.h"

#ifndef WIN

typedef void (*COM_init_func)(MEM_handle handle,MEM_handle messanger);
typedef void (*COM_update_func)(MEM_handle handle,MEM_handle messanger,size_t dt);
typedef void (*COM_destroy_func)(MEM_handle handle,MEM_handle messanger);

typedef struct COM_component COM_component;
typedef struct COM_data COM_data;

struct COM_component{
    COM_init_func init_func;
    COM_update_func update_func;
    COM_destroy_func destroy_func;
};

struct COM_data{
    MEM_heap* lib_handles;
    MEM_handle sdl_wrapper_data;
};



void COM_init(MEM_heap_manager* manager){
    COM_data* data= &MEM_get_item_m(COM_data,MEM_get_heap(manager,MEM_LOC_COM_DATA),0);
    data->lib_handles = MEM_get_heap(manager,MEM_LOC_COM_LIB_HANDLES);
    data->sdl_wrapper_data = MEM_create_handle_from_manager(manager,MEM_LOC_COM_DATA,0);
    int i;
    for(i=0;i<com_data->lib_handles->capacity;i++){
    }
}

/*
int main(int argc,char ** argv){
   void *handle;
    int (*func)(int,int);
    handle = dlopen("libtest.dylib",RTLD_LAZY);
    func = dlsym(handle,"add");
    printf("%d",(*func)(1,5));
    dlclose(handle);
}
*/
#endif


