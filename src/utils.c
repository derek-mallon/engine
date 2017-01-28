#include "utils.h"
#include <string.h>
#include <stdarg.h>

void UTI_concat(char* buff,size_t count,...){
    va_list ap;
    va_start(ap,count);
    int i;
    for(i=0;i<count;i++){
        if(i==0){
            strcpy(buff,va_arg(ap,char*));
        }else{
            strcat(buff,va_arg(ap,char*));
        }
    }
    va_end(ap);

}


