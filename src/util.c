#include "util.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
void append_to_file(char* file_name,char* content){
    FILE* file = fopen(file_name,"a+");
    fputs(content,file);
    fputs("\n",file);
    fclose(file);
}
void log_func(char* content,uint16_t line,char* file){
    time_t rawtime;
    time(&rawtime);
    char buf[255];
    sprintf(buf,"%s=>%s: log at line %d: %s ",asctime(localtime(&rawtime)),file,line,content);
    append_to_file(LOG_PATH,buf);
}
char* read_file(char* path){
    FILE* file = fopen(path,"r");
    if(file == NULL){
        LOG("file to be read not found");
        fclose(file);
        return NULL;
    }
    fseek(file,0,SEEK_END);
    int length = ftell(file);
    fseek(file,0,SEEK_SET);
    char* line = (char*) malloc(length);
    fread(line,1,length,file);
    fclose(file);
    return line;
}
