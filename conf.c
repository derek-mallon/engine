#include "file_handling.h"
#include <string.h>



void CONF_open(const char* path,void(*walk)(const char*,const char*,void*,int),void* data){
        char buff[256],prev[256];
        FILE* file = fopen(path,"r");
        ERR_ASSERT(file != NULL,"null file");
        int count = 0;
        int line = 0;
        while(fscanf(file," %255s",buff) == 0){
            if(count % 2 == 0){
                line++;
            }else{
                walk(prev,buff,data,line);
            }
            strcpy(prev,buff);
        }
}
