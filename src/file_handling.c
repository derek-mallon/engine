#include "file_handling.h"
#include "string.h"
#include "all.h"
#include <string.h>
#ifdef UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif
#ifdef WIN
#include <windows.h>
#endif

FIL_path FIL_create_path(UTI_str path,FIL_file_type type,uint32_t mode){
    char ops[FIL_OPTIONS_SIZE];
    if(mode & FIL_MODE_WRITE && mode & FIL_MODE_READ){
        if(mode & FIL_MODE_BINARY || type == FIL_TYPE_BINARY){
            if(mode & FIL_MODE_OVERWRITE){
                UTI_concat(ops,1,"w+b");
            }else{
                UTI_concat(ops,1,"r+b");
            }
        }else if(mode & FIL_MODE_OVERWRITE){
            UTI_concat(ops,1,"w+");
        }else{
            UTI_concat(ops,1,"r+");
        }
    }else if(mode & FIL_MODE_WRITE){
        if(mode & FIL_MODE_BINARY || type == FIL_TYPE_BINARY){
            if(mode & FIL_MODE_OVERWRITE){
                UTI_concat(ops,1,"wb");
            }else{
                UTI_concat(ops,1,"ab");
            }
        }else if(mode & FIL_MODE_OVERWRITE){
                UTI_concat(ops,1,"w");
        }else{
                UTI_concat(ops,1,"a");
        }
    }else if(mode & FIL_MODE_READ){
        if(mode & FIL_MODE_BINARY || type == FIL_TYPE_BINARY){
            UTI_concat(ops,1,"rb");
        }else{
            UTI_concat(ops,1,"r");
        }
    }else{
        UTI_concat(ops,1,"r");
    }
    FIL_path p;
    UTI_concat(p.ops,1,ops);
    UTI_concat(p.raw,1,path);
    p.extension = strchr(path,'.');
    p.type = type;
    p.mode = mode;
    return p;
}

ERR_error FIL_file_open(FIL_path* p){
    p->file = fopen(p->raw,p->ops);
    if(p->file){
        return ERR_GOOD;
    }
    return ERR_BAD;
}

void FIL_file_close(FIL_path* p){
    ERR_ASSERT((p->file != NULL),"null file")
    fclose(p->file);
}


size_t FIL_file_size_binary(FIL_path* p){
    ERR_ASSERT((p->file != NULL),"null file")
    size_t size;
    fseek(p->file,0,SEEK_END);
    size = ftell(p->file);
    fseek(p->file,0,SEEK_SET);
    return size;

}

void FIL_mkdir(UTI_str p){
#ifdef UNIX
    ERR_ASSERT(access(p,F_OK) == -1,"directory already exists")
    mkdir(p,0777);
#endif
#ifdef WIN
    ERR_ASSERT(PathFileExist(p) == FALSE,"directory already exists") 
    CreateDirectory(p,NULL);
#endif
}

uint8_t FIL_file_exits(UTI_str p){
#ifdef UNIX
    if(access(p,F_OK) != -1){
        return 1;
    }
    return 0;
#endif
#ifdef WIN
    return PathFileExist(p);
#endif
}

uint8_t FIL_file_is_dir(UTI_str p){
#ifdef UNIX
    if(p[strlen(p)-1] != '/'){
        char buff[FIL_MAX_PATH_SIZE];
        UTI_concat(buff,2,p,"/");
        return(FIL_file_exits(buff));
    }
    return(FIL_file_exits(p));
#endif
#ifdef WIN
    return (GetFileAttriutes(p) == FILE_ATTRIBUTE_DIRECTORY);
#endif
}

void FIL_remove_file(UTI_str p){
    ERR_ASSERT(FIL_file_exits(p),"file %s does not exist",p) 
    ERR_ASSERT(!FIL_file_is_dir(p),"file %s is directory",p)
    remove(p);
}

ERR_error FIL_get_all_files(UTI_str p,MEM_heap* heap_of_paths){
#ifdef UNIX
    ERR_ASSERT(FIL_file_is_dir(p),"file %s is not a directory",p);
    DIR* directory;
    struct dirent *entry;
    if((directory = opendir(p)) != NULL){
        while((entry = readdir(directory)) != NULL){
            if(!FIL_file_is_dir(entry->d_name)){
                size_t index;
                ERR_error result;
                if((result = MEM_next_free_item(heap_of_paths,&index)) != ERR_GOOD){
                    return result;
                }
                UTI_concat(MEM_get_item_m(UTI_buff_stor,heap_of_paths,index).buff,1,entry->d_name);
            }
        }
        closedir (directory);
        return ERR_GOOD;
    }
    return ERR_MISSING_FILE;
#endif
#ifdef WIN
    ERR_ASSERT(false,"UNIMPLMENTED CODE!")
#endif
}

ERR_error FIL_read_binary(FIL_path* path,MEM_handle handle){
    ERR_ASSERT(path->file != NULL,"file with path %s not opened",path->raw);
    ERR_ASSERT(path->mode & FIL_MODE_READ,"file with path %s is not readable, the files mode is %s",path->raw,path->ops);
    size_t result = fread(MEM_get_item_m_p(void,handle.heap,handle.index),handle.heap->size_of_object,1,path->file);
    if(result != handle.heap->size_of_object){
        return ERR_BAD;
    }
    return ERR_GOOD;
}
ERR_error FIL_write_binary(FIL_path* path,MEM_handle handle){
    ERR_ASSERT(path->file != NULL,"file with path %s not opened",path->raw);
    ERR_ASSERT(path->mode & FIL_MODE_WRITE,"file with path %s is not writable, the files mode is %s",path->raw,path->ops);
    size_t result = fwrite(MEM_get_item_m_p(void,handle.heap,handle.index),handle.heap->size_of_object,1,path->file);
    if(result != handle.heap->size_of_object){
        return ERR_BAD;
    }
    return ERR_GOOD;
}
