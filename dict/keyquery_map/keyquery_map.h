#ifndef KEYQUERY_MAP_H
#define KEYQUERY_MAP_H
#include <stdbool.h>
#include <stddef.h>
#include "../node/node.h"

typedef struct KEYQUERY_MAP KEYQUERY_MAP;

KEYQUERY_MAP *keyquery_init(size_t capacity);

bool keyquery_put(KEYQUERY_MAP *kq_map, char *table, char *query_key);
bool keyquery_remove(KEYQUERY_MAP *kq_map, char *table);
NODE *keyquery_get(KEYQUERY_MAP *kq_map, char *table);

void keyquery_free(KEYQUERY_MAP *);

#endif
