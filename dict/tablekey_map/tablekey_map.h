#ifndef TABLEKEY_MAP_H
#define TABLE_KEY_MAP_H

#include <stdbool.h>
#include "../node/node.h"
#include "../dict.h"

/**
 * Define incomplete types for encapsulation.
 */

typedef struct TABLEKEY_MAP TABLEKEY_MAP;

typedef struct {
    NODE *tables;
    char *query;
} KEY_INFO;

TABLEKEY_MAP *tablekey_init(size_t capacity);

bool tablekey_put(TABLEKEY_MAP *tk_map, char *query_key, char *table);
bool tablekey_set_query(TABLEKEY_MAP *tk_map, char *query_key, char *query);

bool tablekey_remove(TABLEKEY_MAP *tk_map, char *key);
KEY_INFO *tablekey_get(TABLEKEY_MAP *tk_map, char *key);

void tablekey_free(TABLEKEY_MAP *);



#endif
