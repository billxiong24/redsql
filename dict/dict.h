/**
 * Custom dictonary implementation 
 */
#ifndef DICT_H
#define DICT_H
#include <stdbool.h>

/* included for size_t */
#include <stddef.h>

/**
 * Use incomplete struct type to encaspulate struct members.
 * This struct definition is in _priv_dict.h
 */
typedef struct DICT DICT;

DICT *dict_init(size_t size);

/**
 * If key does not exist, add it to map, otherwise update the key to the new value.
 * @return true if key exists, false otherwise.
 */
bool dict_put(DICT *, char *key, void *val);
bool dict_remove(DICT *, char *key);
void *dict_get(DICT *, char *key);
size_t dict_size(DICT *);

void dict_free(DICT *, void (*free_func) (void *));

#endif
