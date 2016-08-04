#include "util.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
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
struct Frame{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
};
struct Sprite{
     struct Frame* frames;
};
enum ParseState{
    READ_TYPE,
    READ_VALUE,
    READ_NAME,
};
char** strsplit(char* buf,char* delim,size_t* number){
    char** result = NULL;
    char* check = buf;
    char* check_delim = delim;
    char* word = NULL;
    char* buf_cpy = malloc(sizeof(char)*strlen(buf));
    size_t count =0;
    size_t i = 0;
    size_t length = 0;
    bool in_delim = false;
    while(*check){
        check_delim = delim;
        while(*check_delim){
            if(*check == *check_delim && !in_delim){
                in_delim = true;
                break;
            }
            if(*check == *check_delim && in_delim){
                break;
            }
            if(*check != *check_delim && in_delim){
                count++;
                in_delim = false;
                break;
            }
            check_delim++;
        }
        check++;
    }
    result = malloc(sizeof(char*)*count);
    strcpy(buf_cpy,buf);
    word = strtok(buf_cpy,delim);
    while(word){
        length = strlen(word);
        result[i] = malloc(sizeof(char)* length);
        strcpy(result[i],word);
        word = strtok(NULL,delim);
        i++;
    }
    free(buf_cpy);
    *number = count;
    return result;
}
/*
void* parse_data(char* path){
    char* file = read_file(path);
    char* check = file;
    int i = 0;
    enum ParseState parse_state = READ_TYPE;
    while(*check != '\0'){
        i++;
        switch(*check){
            case '{':
                break;
            case '}':
                break;
            case
        }
        check++;
    }
    return NULL;
}
*/
