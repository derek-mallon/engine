#include "mem.h"

#ifndef WIN

typedef void (*COM_init)(MEM_handle handle,MEM_handle messanger);

int main(int argc,char ** argv){
   void *handle;
    int (*func)(int,int);
    handle = dlopen("libtest.dylib",RTLD_LAZY);
    func = dlsym(handle,"add");
    printf("%d",(*func)(1,5));
    dlclose(handle);
}
#endif


