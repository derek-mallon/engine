#ifndef PROJ_CONF_H
#define PROJ_CONF_H
#include "utils.h"

#define ROOT_DATA "PROJECT_CONF.data"

typedef struct PRJ_project_conf PRJ_project_conf;
struct PRJ_project_conf{
    UTI_buff_stor project_name;
    UTI_buff_stor base_path;
    UTI_buff_stor texture_dir;
    UTI_buff_stor component_dir;
    UTI_buff_stor bin_dir;
};

PRJ_project_conf PRJ_default_conf(UTI_str project_name);

void PRJ_create_proj(PRJ_project_conf* conf);


#endif
