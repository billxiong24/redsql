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

char** str_util_split(char* a_str, const char a_delim) {
    char **result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp) {
        if (a_delim == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
     *        knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result) {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token) {
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        *(result + idx) = 0;
    }

    return result;
}
