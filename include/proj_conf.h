#ifndef PROJ_CONF_H
#define PROJ_CONF_H
#include "utils.h"
#include "mem.h"

#define PRJ_ROOT_DATA "PROJECT_CONF.data"

typedef struct PRJ_project_conf PRJ_project_conf;
typedef struct PRJ_component_mem_templates PRJ_component_mem_templates;
typedef struct PRJ_data_block PRJ_data_block;

typedef enum PRJ_data_type PRJ_data_type;
typedef struct PRJ_data_struct PRJ_data_struct;

enum PRJ_data_type{
    PRJ_int,
    PRJ_float,
    PRJ_uint32_t,
    PRJ_UTI_buff_str
};

struct PRJ_data_block{
    UTI_buff_stor data;
    UTI_buff_stor name;
};

struct PRJ_data_struct{
    MEM_heap blocks;
};

struct PRJ_project_conf{
    UTI_buff_stor project_name;
    UTI_buff_stor base_path;
    UTI_buff_stor texture_dir;
    UTI_buff_stor component_dir;
    UTI_buff_stor audio_dir;
    UTI_buff_stor bin_dir;
    UTI_buff_stor mem_binary;
    UTI_buff_stor self;
};


PRJ_project_conf PRJ_default_conf(UTI_str project_name);

void PRJ_create_proj(PRJ_project_conf* conf);

void PRJ_create_proj_mem();

void PRJ_create_proj_binary(PRJ_project_conf conf);

ERR_error PRJ_load_proj_conf(UTI_str path,PRJ_project_conf* conf);

PRJ_data_struct PRJ_create_data_struct();

void PRJ_add_data_block_(PRJ_data_struct* structure,UTI_str name,PRJ_data_type,size_t index);

void* PRJ_get_data_value(const PRJ_data_struct* structure,size_t index);

#define PRJ_cast(type) *(type*)

#define PRJ_add_data_block(field,type) PRJ_add_data_block_(structure,fields[field],PRJ_##type,field)

#define PRJ_set_data_block(field,type) PRJ_cast(type)PRJ_get_data_value(structure,field)

#endif
