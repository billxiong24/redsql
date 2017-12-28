#include <stdio.h>
#include "../dict.h"
#include "../dirtybit_map.h"
#include <string.h>


void test(DICT *dict, char *key, char *v) {
    unsigned char *p = v;
    dict_put(dict, key, p);
}

void str(DICT *dict, char *key, char *val) {
    /*char buf[16];*/
    dict_put(dict, key, val);
}

int main(void) {

    DBM *dict = dbm_init(16);

    dbm_put(dict, "asdf", 0);
    dbm_put(dict, "dfdf", 1);
    dbm_put(dict, "vbvb", 1);
    dbm_put(dict, "hithere", 1);
    printf("dbm_get(dict) = %d\n", dbm_get(dict, "vbvb"));

    dbm_free(dict);
    return 0;
}
