#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <stdlib.h>

#define HAS_PRIME 2999

typedef struct HAS_hashtable HAS_hashtable;
typedef struct HAS_item HAS_item;

struct HAS_item{

};

struct HAS_hashtable{
    size_t indexes[HAS_PRIME];

};

size_t HAS_create_hash(const char* key);


#endif
