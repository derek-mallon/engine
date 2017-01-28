#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H
#include <stdio.h>
#include "error.h"


size_t FIL_file_size_binary(const char* p);

void FIL_mkdir(const char* p);

uint8_t FIL_file_exits(const char* p);

uint8_t FIL_file_is_dir(const char* p);

void FIL_remove_file(const char* p);

size_t FIL_get_number_of_files_in_dir(const char* p);

size_t FIL_get_number_of_dirs_in_dir(const char* p);

ERR_error FIL_read_binary(const char* path,void* data,size_t size);

ERR_error FIL_write_binary(const char* path,void* heap,size_t size);

ERR_error FIL_walk_over_all_files_in_dir(const char* dir,void(*walk)(const char*,void*,int),void* data);
#endif
