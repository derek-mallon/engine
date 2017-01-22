#ifndef MESSAGING_H
#define MESSAGING_H
#include "mem.h"
typedef enum MES_type MES_type;

typedef struct MES_message MES_message;
typedef union MES_message_data MES_message_data;
typedef struct MES_comp_other MES_comp_other;
typedef struct MES_comp_self MES_comp_self;
typedef struct MES_messanger MES_messanger;


enum MES_type{
    MEM_NONE,
    MES_COMP_DESTROY,
    MES_COMP_CREATE,
    MES_COMP_SELF,
    MES_COMP_OTHER,
};

struct MES_comp_self{
    uint32_t type;
    MEM_handle data;
};

struct MES_comp_other{
    uint32_t comp_type;
    uint32_t type;
    MEM_handle data;
};

union MES_message_data{
    MES_comp_self comp_self;
    MES_comp_other comp_other;
};

struct MES_message{
    MES_message_data data;
    MES_type type;
};

struct MES_messanger{
};

void MES_send_message(MEM_handle target,size_t type_offset,size_t type);



#endif
