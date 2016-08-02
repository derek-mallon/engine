#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <hashtable.h>
#define LOG(content) log_func((content),__LINE__,__FILE__)
#define LOG_PATH "log.txt"
void append_to_file(char* file_name,char* content);
void log_func(char* content,uint16_t line,char* file);
char* read_file(char* path);
Hashtable parse_data(char* path);
#endif
