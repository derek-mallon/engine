#ifndef ERROR_H
#define ERROR_H
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum ERR_error ERR_error;


enum ERR_error{
    ERR_GOOD,
    ERR_BAD,
    ERR_BAD_ARG,
    ERR_MISSING_FILE,
    ERR_MEM,
};

#ifndef DEBUG
#define ERR_HANDLE(exp)  if((result = exp) != ERR_GOOD){return result;}
#endif

#ifdef DEBUG
#define ERR_HANDLE(exp)  if((result = exp) != ERR_GOOD){\
    printf("ERROR CAUGHT IN FILE:%s  FUNC:%s LINE:%d \n",__FILE__,__func__,__LINE__);\
    return result;\
}

#endif

#ifdef DEBUG
#define ERR_ASSERT(exp,fmt,...) \
do{\
    if(!(exp)){\
        fprintf(stderr,fmt,##__VA_ARGS__);\
        printf("\n");\
        printf("ASSERT OCCURRED IN FILE:%s  FUNC:%s LINE:%d \n",__FILE__,__func__,__LINE__);\
        exit(1);\
    }\
}while(0) //forces the complier to require a semicolon with no overhead
#define ERR_ASSERT_BLOCK(code) code

#endif
#ifndef DEBUG
#define ERR_ASSERT(exp,fmt,...) do { } while(0)
#define ERR_ASSERT_BLOCK(code)
#endif

#endif
