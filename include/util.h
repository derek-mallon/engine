#ifndef UTIL_H
#define UTIL_H
#include <stdint.h>
#include <hashtable.h>
#include <SDL2/Sdl.h>
#define LOG(content) log_func((content),__LINE__,__FILE__)
#define LOG_PATH "log.txt"
#define  MAX_TEXTURES 10 
#define DEFAULT_FONT_SIZE 20
#define VA_NUM_ARGS(...) VA_NUM_ARGS_IMPL(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1)
#define VA_NUM_ARGS_IMPL(_1,_2,_3,_4,_5,_6,_8,_9,_10,N,...) N
typedef enum {false,true} bool;
void append_to_file(char* file_name,char* content);
void log_func(char* content,uint16_t line,char* file);
char* read_file(char* path);
char** strsplit(char* buf,char* delim,size_t* number);
bool load_texture_from_image(SDL_Renderer* renderer,char* path,SDL_Texture* texture);
bool load_texture_from_font(SDL_Renderer* renderer,char* path,SDL_Texture* texture);
typedef struct RenderInfo{
    SDL_Rect frhme;
    SDL_Point pos;
    double angle;
    SDL_Point center;
    size_t texture;
}RenderInfo;
#endif
