#include "str_util.h"


char *str_util_clone(char *data) {
    if(!data) {
        return NULL;
    }
    size_t len_key = strlen(data);
    char *src = malloc(sizeof(char) * (len_key + 1));
    strncpy(src, data, len_key);
    src[len_key] = '\0';

    return src;
}
