#include "dirtybit_map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DBM {
    DICT *dict;
};

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

    if(val == 1) {
        return dict_put(dict, key, "1");
    }
    else {
        return dict_put(dict, key, "0");
    }
}

bool dbm_remove(DBM *dbm, char *key) {
    return false;
}

unsigned char dbm_get(DBM *dbm, char *key) {
    DICT *dict = dbm->dict;
    unsigned char *ptr = dict_get(dict, key);

    if(ptr == NULL) {
        return DBM_NULL;
    }
    return atoi((const char *) ptr);
}

static void set_bit_pattern(DICT *dict, char *key, char *pattern) {
    char *res = strstr(key, pattern);
    if(key - res == 0) {
        dict_put(dict, key, "1");
    }
}

void dbm_set_pattern_val(DBM *dbm, char *pattern) {
    DICT *dict = dbm->dict;
    dict_for_each(dict, pattern, set_bit_pattern);
}

void dbm_free(DBM *dbm) {
    dict_free(dbm->dict, NULL);
    free(dbm);
}
