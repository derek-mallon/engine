#ifndef UTILS_H
#define UTILS_H
#include <stdint.h>
#include <stdio.h>
#include "all.h"


typedef const char* UTI_str;
typedef char* UTI_buff;

void UTI_concat(UTI_buff buff,size_t count,...);

#endif

