#include <stdio.h>
#include "../keyquery_map/keyquery_map.h"
#include "../tablekey_map/tablekey_map.h"
#include <string.h>
#include <stdlib.h>

void func(KEYQUERY_MAP *dict) {
    /*keyquery_put(dict, "empl", "k1");*/
    /*keyquery_put(dict, "empl", "wat");*/
    /*keyquery_put(dict, "empl", "wat2");*/
    /*keyquery_put(dict, "empl", "wat3");*/
    /*keyquery_put(dict, "empl", "wat4");*/
    char c[16];
    strncpy(c, "sec", 16);
    char d[16];
    strncpy(d, "wa5", 16);
    keyquery_put(dict, c, d);
    keyquery_put(dict, "sec", "wat6");
    keyquery_put(dict, "sec", "wat7");
}

void tk(TABLEKEY_MAP *tk_map) {
    char buf[16];
    strncpy(buf, "k2", 3);
    char query[64];
    strncpy(query, "select * from User", 60);
    tablekey_put(tk_map, buf, "last");
    tablekey_put(tk_map, buf, "wat");
    tablekey_put(tk_map, buf, "3434");
    tablekey_set_query(tk_map, buf, query);
}

int main(void) {

    KEYQUERY_MAP *dict = keyquery_init(16);
    TABLEKEY_MAP *tk_map = tablekey_init(16);
    tablekey_put(tk_map, "k1", "employee");
    tablekey_put(tk_map, "k1", "salary");
    tablekey_put(tk_map, "k1", "adfkj");
    tablekey_put(tk_map, "k1", "last");

    tk(tk_map);

    KEY_INFO *info = tablekey_get(tk_map, "k2");
    puts(info->query);


    NODE *trav = info->tables;

    while(trav) {
        puts(trav->val);
        trav = trav->next;
    }

    keyquery_free(dict);

    return 0;
}
