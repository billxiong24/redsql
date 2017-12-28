/**
 * Do not include this file anywhere. This header file is intended for implementation detail
 * in conjunction with dict.h
 */
#ifndef _PRIV_DICT_H
#define _PRIV_DICT_H

typedef struct NODE {
    struct NODE *next;
    char *key;
    char *val;
} NODE;

struct DICT {
    size_t size;
    NODE **arr;
};

#endif
