#ifndef TERM_H
#define TERM_H

#include "stdio.h" 
#define TERM_printlnf(fmt,...) printf(fmt,__VA_ARGS__);printf("\n")

#define TERM_println(str) printf(str); printf("\n")

#define TERM_print(str) printf(str)

#define TERM_get_int(i) scanf("%d",&i)

#define TERM_get_str(str) scanf("%s",str)

#define TERM_get_float(f) scanf("%f",&f)
#endif
