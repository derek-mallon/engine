#include "heap.h"
#include "utils.h"

typedef struct resource_manager{
    chunk texture_paths;
    chunk sprite_meta_data_paths;
    chunk textures;
}resource_manager;

