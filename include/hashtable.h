#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "mem.h"

#define HAS_PRIME 2999

typedef struct HAS_hashtable HAS_hashtable;

struct HAS_hashtable{
    size_t indexes[HAS_PRIME];

};

HAS_create_hash();


#endif
