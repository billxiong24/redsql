#include "node.h"

void node_free(NODE *node) {
    while(node) {
        NODE *temp = node;
        node = node->next;
        free(temp);
    }
}
