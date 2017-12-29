#include <stdio.h>
#include "../keyquery_map/keyquery_map.h"
#include <string.h>
#include <stdlib.h>


int main(void) {

    KEYQUERY_MAP *dict = keyquery_init(16);
    keyquery_put(dict, "empl", "k1");
    keyquery_put(dict, "empl", "wat");
    keyquery_put(dict, "empl", "wat2");
    keyquery_put(dict, "empl", "wat3");
    keyquery_put(dict, "empl", "wat4");
    keyquery_put(dict, "sec", "wat4");
    keyquery_put(dict, "sec", "wat5");
    keyquery_put(dict, "sec", "wat6");
    keyquery_put(dict, "sec", "wat7");

    NODE *node = keyquery_get(dict, "sec");
    NODE *trav = node;

    while(trav) {
        puts(trav->val);

        trav = trav->next;
    }

    keyquery_free(dict);

    return 0;
}
