#include <stdio.h>
#include "../dict.h"

int main(void) {

    DICT *dict = dict_init(16);
    dict_put(dict, "a", "b");
    dict_put(dict, "df", "asdf");
    dict_put(dict, "a", "dfad");
    dict_put(dict, "ef", "aer");
    dict_put(dict, "ghy", "b");

    puts(dict_get(dict, "a"));
    puts(dict_get(dict, "df"));
    puts(dict_get(dict, "ef"));
    puts(dict_get(dict, "ghy"));

    dict_free(dict);
    
    return 0;
}
