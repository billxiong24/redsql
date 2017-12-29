#ifndef NODE_H
#define NODE_H
#include <stdlib.h>

typedef struct NODE {
    struct NODE *next;
    void *val;
} NODE;

void node_free(NODE *);



#endif
