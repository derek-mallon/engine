#include <stdio.h>
#include <string.h>
#include "../lib/lua-5.3.3/src/lua.h"
#include "../lib/lua-5.3.3/src/lualib.h"
#include "../lib/lua-5.3.3/src/lauxlib.h"

uint8_t extract_string(lua_State *L,char* name,const char** str){
    lua_getglobal(L,name);
    if(!lua_isstring(L,-1))
        return 0;
    *str = lua_tostring(L,-1);
    return 1;
}
uint8_t extract_uint16(lua_State *L,char* name,uint16_t* number){
    lua_getglobal(L,name);
    if(!lua_isnumber(L,-1))
        return 0;
    *number = (int)lua_tonumber(L,-1);
    return 1;
}

/*
uint8_t extract_table_of_strings(lua_State *L,char* name,ARRAY(string)* strings){
    return 0;
}
*/

uint8_t pcall_wrapper(lua_State *L,int nargs,int nresults,int msgh){
    if(lua_pcall(L,nargs,nresults,msgh) !=0){
        fprintf(stderr,"ERROR LUA_PCALL %s", lua_tostring(L,-1));
    }
    return 1;
}

/*
uint8_t load_sdl_init(lua_State *L,char* filename,init_sdl_data* init){
    init_sdl_data data;
    if(luaL_loadfile(L,filename) || lua_pcall(L,0,0,0)){
        printf("lua error");
    }
    if(!extract_string(L,"title",&data.title))
        return 0;
    if(!extract_uint16(L,"screen_width",&data.screen_width))
        return 0;
    if(!extract_uint16(L,"screen_height",&data.screen_height))
        return 0;
    if(!extract_table_of_strings(L,"texture_paths",&data.texture_paths))
        return 0;
    return 1;
}
*/
