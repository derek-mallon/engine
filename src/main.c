#include "proj_conf.h"
#include "term.h"
#include <string.h>
int main(int argc,char* argv[]){
   if(argc > 1){
        if(strcmp(argv[1],"--new") == 0){
            PRJ_project_conf conf;
            if(argc >1){
                UTI_buff_stor name;
                TERM_println("Enter project name");
                TERM_get_str(name.buff);
                conf = PRJ_default_conf(name.buff);
            }else{
                conf = PRJ_default_conf("default");
            }
            PRJ_create_proj(&conf);
            
        }else if(strcmp(argv[1],"--build") == 0){
            PRJ_project_conf* conf;
            PRJ_create_proj_binary(conf);
        }
    }else{
        TERM_println("USAGE: ");
        TERM_println("\tproject --new creates a new project");
        TERM_println("\tproject --new default creates a new project");
        TERM_println("\tproject --build builds a project if found");
    }

    
}




