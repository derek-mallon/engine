#include "heap.h"

typedef struct node{
    struct node *north,*east,*south,*west;
    float x,y;
}node;

typedef struct graph{
    heap* node_heap;
    size_t start,end;
}graph;

typedef struct gragh_system{
    heap* graph_heap;
    size_t start,end;
}graph_system;

int main(void){
}
