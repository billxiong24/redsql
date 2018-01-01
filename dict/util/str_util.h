
/**
 * Defines common string functions used.
 */
#ifndef STR_UTIL_H
#define STR_UTIL_H
#include <string.h>
#include <stdlib.h>


char *str_util_clone(char *data);
char** str_util_split(char* str, const char delim);


#endif
