#include "dirtybit_map.h"
#include <stdio.h>
#include <stdlib.h>

struct DBM {
    DICT *dict;
};

static void free_func(void *val) {
    char *v = val;
    free(v);
}

DBM *dbm_init(size_t capacity) {
    DBM *dbm = malloc(sizeof(*dbm));
    dbm->dict = dict_init(capacity);

    return dbm;
}

unsigned char dbm_put(DBM *dbm, char *key, unsigned char val) {

    DICT *dict = dbm->dict;
    if(val != 0 && val != 1) {
        return DBM_RANGE_ERR;
    }

    /**
     * have to malloc 1 byte otherwise we have pointer to arg var
     * on stack...:(
     */
    unsigned char *ptr = malloc(sizeof(unsigned char) * 1);
    *ptr = val;
    dict_put(dict, key, ptr);
}

bool dbm_remove(DBM *dbm, char *key) {
    return false;
}

unsigned char dbm_get(DBM *dbm, char *key) {
    DICT *dict = dbm->dict;
    unsigned char *ptr = dict_get(dict, key);
    char *res = dict_get(dict, key);

    if(ptr == NULL) {
        return DBM_NULL;
    }
    return *ptr;
}

void dbm_free(DBM *dbm) {
    dict_free(dbm->dict, free_func);
    free(dbm);
}
