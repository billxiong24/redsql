#ifndef NODE_H
#define NODE_H
#include <stdlib.h>

typedef struct NODE {
    struct NODE *next;
    char *val;
} NODE;

void node_free(NODE *);

NODE *node_init(char *val);



#endif
