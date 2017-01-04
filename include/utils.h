#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stdio.h>

#define UTI_DEFAULT_NAME_SIZE 50

typedef const char* UTI_str;
typedef char* UTI_buff;
typedef struct UTI_buff_stor UTI_buff_stor;

struct UTI_buff_stor{
    char buff[UTI_DEFAULT_NAME_SIZE];
};

void UTI_concat(UTI_buff buff,size_t count,...);

#endif

