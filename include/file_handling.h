#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include "utils.h"
#include "error.h"
#include "mem.h"

#define FIL_MAX_PATH_SIZE 100

#define FIL_OPTIONS_SIZE 4

#define FIL_MODE_WRITE 0x01
#define FIL_MODE_READ 0x02
#define FIL_MODE_BINARY 0x04
#define FIL_MODE_OVERWRITE 0x08

typedef FILE* FIL_file;

typedef struct FIL_path FIL_path;

typedef enum FIL_file_type FIL_file_type;


enum FIL_file_type{
    FIL_TYPE_BINARY,
    FIL_TYPE_TEXT,
    FIL_TYPE_DIR,
    FIL_TYPE_LIB,
    FIL_TYPE_UNKNOWN,
};

struct FIL_path{
    char raw[FIL_MAX_PATH_SIZE];
    UTI_str extension;
    char ops[FIL_OPTIONS_SIZE];
    FIL_file_type type;
    FIL_file file;
    uint32_t mode;
};


FIL_path FIL_create_path(UTI_str path,FIL_file_type type,uint32_t mode);

ERR_error FIL_file_open(FIL_path* p);

void FIL_file_close(FIL_path* p);

size_t FIL_file_size_binary(FIL_path* p);

void FIL_mkdir(UTI_str p);

uint8_t FIL_file_exits(UTI_str p);

uint8_t FIL_file_is_dir(UTI_str p);

void FIL_remove_file(UTI_str p);

size_t FIL_get_number_of_files_in_dir(UTI_str p);

ERR_error FIL_get_all_files(UTI_str p,MEM_heap* heap_of_paths);

size_t FIL_get_number_of_dirs_in_dir(UTI_str p);

ERR_error FIL_get_all_dirs(UTI_str p,MEM_heap* heap_of_paths);

ERR_error FIL_read_binary(FIL_path* path,MEM_handle handle);

ERR_error FIL_write_binary(FIL_path* path,MEM_handle handle);

ERR_error FIL_walk_over_all_files_in_dir(UTI_str dir,void(*walk)(UTI_str,void*),void* data);
#endif
