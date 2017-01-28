#include "proj_conf.h"
#include "term.h"
#include "all.h"
#include <string.h>
#include "engine.h"
#include "module_startup_shutdown.h"

void print_help(){
        TERM_println("USAGE: ");
        TERM_println("\tproject --new default creates a new project");
}
int main(int argc,char* argv[]){
   if(argc > 1){
        if(strcmp(argv[1],"--new") == 0)
            PRJ_create_proj();
    }else{
        print_help();
    }
}




