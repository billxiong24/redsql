/**
 * Do not include this file anywhere. This header file is intended for implementation detail
 * in conjunction with dict.h
 */
#ifndef _PRIV_DICT_H
#define _PRIV_DICT_H

typedef struct DICT_NODE {
    struct DICT_NODE *next;
    char *key;
    void *val;
} DICT_NODE;

struct DICT {
    size_t size;
    DICT_NODE **arr;
};

#endif
