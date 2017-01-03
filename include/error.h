#ifndef ERROR_H
#define ERROR_H
#include <assert.h>
#include <stdio.h>
typedef enum ERR_error ERR_error;

enum ERR_error{
    ERR_GOOD,
    ERR_BAD,
    ERR_BAD_ARG,
    ERR_MISSING_FILE,
    ERR_MEM,
};

#ifdef DEBUG
#define ERR_ASSERT(exp,fmt,...) if(!(exp)){\
    fprintf(stderr,fmt,__VA_ARGS__);\
    printf("ASSERT OCCURRED IN FILE:%s  FUNC:%s LINE:%d \n",__FILE__,__func__,__LINE__);\
    exit(1);\
}
#endif
#ifndef DEBUG
#define ERR_ASSERT(exp,fmt,...)
#endif

#endif
