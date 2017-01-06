#include "proj_conf.h"
#include "file_handling.h"

PRJ_project_conf PRJ_default_conf(UTI_str project_name){
    PRJ_project_conf conf;
    UTI_concat(conf.base_path.buff,2,"./",project_name);
    UTI_concat(conf.texture_dir.buff,2,conf.base_path.buff,"/textures");
    UTI_concat(conf.component_dir.buff,2,conf.base_path.buff,"/components");
    UTI_concat(conf.bin_dir.buff,2,conf.base_path.buff,"/bin");
    return conf;
}

void PRJ_create_proj(PRJ_project_conf* conf){
   FIL_mkdir(conf->base_path.buff);
   FIL_mkdir(conf->texture_dir.buff);
   FIL_mkdir(conf->component_dir.buff);
   FIL_mkdir(conf->bin_dir.buff);
}



#ifdef PROJECT_CONF
int main(int argc,char* argv[]){
    PRJ_project_conf conf;
    if(argc >1){
        conf = PRJ_default_conf(argv[1]);
    }else{
        conf = PRJ_default_conf("default");
    }
    PRJ_create_proj(&conf);
}

#endif
