/**
 * Defines a map to keep track of dirty bits for each key/query in redis,
 * so we know whether or not data is stale.
 */
#ifndef DIRTYBIT_MAP_H
#define DIRTYBIT_MAP_H
#include "../dict.h"

#define DBM_NULL 2
#define DBM_RANGE_ERR 3

typedef struct DBM DBM;

DBM *dbm_init(size_t capacity);

unsigned char dbm_put(DBM *, char *key, unsigned char val);
bool dbm_remove(DBM *, char *key);
unsigned char dbm_get(DBM *, char *key);

void dbm_set_pattern_val(DBM *, char *pattern);

void dbm_free(DBM *);

#endif
