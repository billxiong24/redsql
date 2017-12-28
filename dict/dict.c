#include "dict.h"
#include <stdlib.h>
#include <string.h>
#include "_priv_dict.h"

static unsigned int hash(char *str, size_t size) {
    unsigned long hash = 5857;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash % size;
}

static NODE *new_node(char *key, char *val) {
    NODE *n = malloc(sizeof(NODE));
    n->next = NULL;
    n->key = key;
    n->val = val;
    return n;
}

DICT *dict_init(size_t size) {
    DICT *dict = malloc(sizeof(DICT));
    dict->size = size;
    dict->arr = malloc(sizeof(NODE *) * size);

    for (int i = 0; i < size; i++) {
        dict->arr[i] = NULL;
    }

    return dict;
}

bool dict_put(DICT *dict, char *key, void *val) {
    unsigned int ind = hash(key, dict->size);
    NODE *head = dict->arr[ind];
    if(!head) {
        dict->arr[ind] = new_node(key, val);
        return false;
    }

    //check if key is in map
    NODE *trav = head;
    while(trav) {

        //key found
        if(strcmp(trav->key, key) == 0) {
            trav->val = val;
            return true;
        }

        trav = trav->next;
    }

    //key wasnt found in map
    NODE *node = new_node(key, val);
    node->next = head;
    dict->arr[ind] = node;
    return true;
}

bool dict_remove(DICT *dict, char *key) {
    return false;
}

void *dict_get(DICT *dict, char *key) {
    unsigned int ind = hash(key, dict->size);
    NODE *head = dict->arr[ind];

    if(!head) {
        return NULL;
    }

    NODE *trav = head;
    while(trav) {
        if(strcmp(key, trav->key) == 0) {
            return trav->val;
        }
        trav = trav->next;
    }

    return NULL;
}

size_t dict_size(DICT *dict) {
    return dict->size;
}

void dict_free(DICT *dict, void (*free_func)(void *)) {
    NODE **arr = dict->arr;
    size_t size = dict->size;

    //free each node before freeing array
    for (int i = 0; i < size; i++) {
        NODE *trav = arr[i];
        while(trav) {
            NODE *temp = trav;
            if(free_func) {
                free_func(trav->val);
            }
            trav = trav->next;
            free(temp);
        }
    }

    free(arr);
    free(dict);
}
