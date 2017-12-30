#include "tablekey_map.h"
#include <stdlib.h>
#include <stdio.h>
#include "../util/str_util.h"

struct TABLEKEY_MAP {
    DICT *dict;
};

static KEY_INFO *new_key_info() {
    KEY_INFO *info = malloc(sizeof(KEY_INFO));
    info->query = str_util_clone(NULL);
    info->tables = NULL;

    return info;
}

static void key_info_free(void *arg) {
    KEY_INFO *val = arg;
    node_free(val->tables);
    free(val);
}

TABLEKEY_MAP *tablekey_init(size_t capacity) {
    TABLEKEY_MAP *tk_map = malloc(sizeof(TABLEKEY_MAP));
    tk_map->dict = dict_init(capacity);

    return tk_map;
}

bool tablekey_put(TABLEKEY_MAP *tk_map, char *query_key, char *table) {
    DICT *dict = tk_map->dict;
    KEY_INFO *info = dict_get(dict, query_key);
    if(!info) {
        info = new_key_info();
        info->tables = node_init(table);
        //key doesnt exist, create new key_info with query and null list of tables
        return dict_put(dict, query_key, info);
    }

    if(!info->tables) {
        //list of tables null, create first node
        info->tables = node_init(table);
    }
    else {
        NODE *n = node_init(table);
        n->next = info->tables;
        info->tables = n;
    }

    return true;
}

bool tablekey_set_query(TABLEKEY_MAP *tk_map, char *query_key, char *query) {
    DICT *dict = tk_map->dict;
    KEY_INFO *info = dict_get(dict, query_key);

    if(!info) {
        return false;
    }

    info->query = str_util_clone(query);
    return true;
}

bool tablekey_remove(TABLEKEY_MAP *tk_map, char *table) {
    return false;
}

KEY_INFO *tablekey_get(TABLEKEY_MAP *tk_map, char *table) {
    return (KEY_INFO *) dict_get(tk_map->dict, table);
}

void tablekey_free(TABLEKEY_MAP *tk_map) {
    dict_free(tk_map->dict, (void (*)(void *)) node_free);
    free(tk_map);
}


