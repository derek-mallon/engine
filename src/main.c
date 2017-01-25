#include "proj_conf.h"
#include "term.h"
#include "io.h"
#include <string.h>
void print_help(){
        TERM_println("USAGE: ");
        TERM_println("\tproject --new creates a new project");
        TERM_println("\tproject --new default creates a new project");
        TERM_println("\tproject --build [location of proj_conf.binary]] builds a project if found at given binary");
}
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

            //save the conf
            FIL_path path = FIL_create_path(conf.self.buff,FIL_TYPE_BINARY,FIL_MODE_WRITE | FIL_MODE_OVERWRITE);
            MEM_heap mem;
            MEM_create_heap(MEM_create_heap_template(PRJ_project_conf,1),&mem);
            MEM_get_item_m(PRJ_project_conf,&mem,0) = conf;
            IO_save_heap_binary(&path,&mem);
            
        }else if(strcmp(argv[1],"--build") == 0){
            if(argc > 2){
                PRJ_project_conf conf;
                PRJ_load_proj_conf(argv[2],&conf);
            }else{
                TERM_println("ERROR no location of proj_conf given");
                print_help();
            }
        }
    }else{
        print_help();
    }

    
}




