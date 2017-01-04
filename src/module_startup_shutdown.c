#include "sdl_wrapper.h"
#include "io.h"
#include "file_handling.h"
#include "mem.h"
#include "utils.h"
typedef enum MOD_heap_types MOD_heap_types;

enum MOD_heap_types{
    MOD_HT_TEXTURE_PTR,
    MOD_HT_END
};

void heap_init(MEM_heap_template* templates){
    templates[MOD_HT_TEXTURE_PTR] = MEM_create_heap_template(MOD_HT_TEXTURE_PTR,10);
}
void init(){
    MEM_heap_manager heap_manager = MEM_create_heap_manager("main",MOD_HT_END,heap_init);


}
