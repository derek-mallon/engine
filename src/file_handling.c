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
#include "utils.h"



size_t FIL_file_size_binary(const char* p){
    FILE* file = fopen(p,"r");
    ERR_ASSERT((file != NULL),"null file");
    size_t size;
    fseek(file,0,SEEK_END);
    size = ftell(file);
    fseek(file,0,SEEK_SET);
    fclose(file);
    return size;
}

void FIL_mkdir(const char* p){
#ifdef UNIX
    ERR_ASSERT(access(p,F_OK) == -1,"directory already exists");
    mkdir(p,0777);
#endif
#ifdef WIN
    ERR_ASSERT(PathFileExist(p) == FALSE,"directory already exists");
    CreateDirectory(p,NULL);
#endif
}

uint8_t FIL_file_exits(const char* p){
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

uint8_t FIL_file_is_dir(const char* p){
#ifdef UNIX
    if(p[strlen(p)-1] != '/'){
        char buff[sizeof(p)+2];
        UTI_concat(buff,2,p,"/");
        return(FIL_file_exits(buff));
    }
    return(FIL_file_exits(p));
#endif
#ifdef WIN
    return (GetFileAttriutes(p) == FILE_ATTRIBUTE_DIRECTORY);
#endif
}

void FIL_remove_file(const char* p){
    ERR_ASSERT(FIL_file_exits(p),"file %s does not exist",p);
    ERR_ASSERT(!FIL_file_is_dir(p),"file %s is directory",p);
    remove(p);
}

size_t FIL_get_number_of_files_in_dir(const char* p){
#ifdef UNIX
    ERR_ASSERT(FIL_file_is_dir(p),"file %s is not a directory",p);
    size_t count = 0;
    DIR* directory;
    struct dirent *entry;
    if((directory = opendir(p)) != NULL){
        while((entry = readdir(directory)) != NULL){
            if(!FIL_file_is_dir(entry->d_name)){
                count++;
            }
        }
        closedir (directory);
    }
    return count;
#endif
#ifdef WIN
    ERR_ASSERT(false,"UNIMPLMENTED CODE!")
#endif
}

ERR_error FIL_walk_over_all_files_in_dir(const char* dir,void(*walk)(const char*,void*,int),void* data){
#ifdef UNIX
    ERR_ASSERT(FIL_file_is_dir(dir),"file %s is not a directory",dir);
    DIR* directory;
    struct dirent *entry;
    int count = 0;
    if((directory = opendir(dir)) != NULL){
        while((entry = readdir(directory)) != NULL){
            if(!FIL_file_is_dir(entry->d_name)){
                walk(entry->d_name,data,count);
                count++;
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

size_t FIL_get_number_of_dirs_in_dir(const char* p){
#ifdef UNIX
    ERR_ASSERT(FIL_file_is_dir(p),"file %s is not a directory",p);
    size_t count = 0;
    DIR* directory;
    struct dirent *entry;
    if((directory = opendir(p)) != NULL){
        while((entry = readdir(directory)) != NULL){
            if(FIL_file_is_dir(entry->d_name)){
                count++;
            }
        }
        closedir (directory);
    }
    return count;
#endif
#ifdef WIN
    ERR_ASSERT(false,"UNIMPLMENTED CODE!")
#endif
}

ERR_error FIL_read_binary(const char* path,void* data,size_t size){
    FILE* file = fopen(path,"r");
    ERR_ASSERT(file != NULL,"file null");
    size_t result = fread(data,size,1,file);
    fclose(file);
    if(result != 1){
        return ERR_BAD;
    }
    return ERR_GOOD;
}
ERR_error FIL_write_binary(const char* path,void* data,size_t size){
    FILE* file = fopen(path,"w");
    ERR_ASSERT(file != NULL,"file null");
    size_t result = fwrite(data,size,1,file);
    fclose(file);
    if(result != 1){
        return ERR_BAD;
    }
    return ERR_GOOD;
}
