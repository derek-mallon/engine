#ifndef SERIALIZE_OUTPUT_H
#define SERIALIZE_OUTPUT_H
#include  "mem.h"
#include "../components/test_comp.h"
static const int number_of_structs = 1;
static const char* struct_array[] = {
	"COMPONENT_test_comp",
};
static const int struct_size[] = {
	sizeof(COMPONENT_test_comp),
};
static const int struct_field_number_array[] = {
	2,
};
static const char* struct_field[1][2] = {
	{
		"x",
                "y",
	},
};
static int number_of_functions = 1;
static const char* function_names[] = {
	"MESSAGE_update",
};
static void(*functions[1])(MEM_handle,MEM_handle,MEM_heap_manager*) = {
	MESSAGE_update,
};
#endif
