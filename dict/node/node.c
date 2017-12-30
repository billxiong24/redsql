#include "node.h"
#include "../util/str_util.h"

NODE *node_init(char *val) {
    NODE *node = malloc(sizeof(*node));
    node->next = NULL;
    node->val = str_util_clone(val);
    return node;
}


void node_free(NODE *node) {
    while(node) {
        NODE *temp = node;
        node = node->next;
        free(temp->val);
        free(temp);
    }
}
