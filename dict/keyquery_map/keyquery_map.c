#include "keyquery_map.h"
#include "../dict.h"
#include <stdlib.h>

struct KEYQUERY_MAP {
    DICT *dict;
};

static NODE *new_node(char *val) {
    NODE *node = malloc(sizeof(*node));
    node->next = NULL;
    node->val = val;
    return node;
}

KEYQUERY_MAP *keyquery_init(size_t capacity) {
    KEYQUERY_MAP *kq_map = malloc(sizeof(KEYQUERY_MAP));
    kq_map->dict = dict_init(capacity);

    return kq_map;
}

bool keyquery_put(KEYQUERY_MAP *kq_map, char *table, char *query_key) {
    DICT *dict = kq_map->dict;

    NODE *node = dict_get(dict, table);
    NODE *add = new_node(query_key);
    if(!node) {
        return dict_put(dict, table, add);
    }

    add->next = node;
    node = add;
    return dict_put(dict, table, node);
}

bool keyquery_remove(KEYQUERY_MAP *kq_map, char *table) {
    return false;
}

NODE *keyquery_get(KEYQUERY_MAP *kq_map, char *table) {
    return dict_get(kq_map->dict, table);
}

void keyquery_free(KEYQUERY_MAP *kq_map) {
    dict_free(kq_map->dict, (void (*)(void *)) node_free);
    free(kq_map);
}
