#ifndef MODULE_STARTUP_SHUTDOWN_H
#define MODULE_STARTUP_SHUTDOWN_H
#include "mem.h"
#include "utils.h"

ERR_error startup(MEM_heap_manager* manager,UTI_str);

void shutdown(MEM_heap_manager* manager);

#endif
