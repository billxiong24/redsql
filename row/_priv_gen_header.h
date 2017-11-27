#ifndef _PRIV_GEN_HEADER_H
#define _PRIV_GEN_HEADER_H

/**
 * This header file is used for "boiler-plate" code generation, in order
 * to define the custom implementations of certain "abstract" methods defined
 * in _priv_row.h. These are functions are then used to populate a vtable, to enable
 * dynamic function binding and give a sense of polymorphism.
 * This header file should not be included in any client program.
 */

#define STRCAT(a, b) a##b

#define ITER_STRUCT(API_NAME) struct STRCAT(API_NAME, _RES_ROWS_ITER *)

#define gen_headers(API_NAME, api_name)\
    struct RES_ROWS;\
    struct STRCAT(API_NAME, RES_ROWS_ITER);\
    \
    \
    struct STRCAT(API_NAME, _RES_ROWS_ITER) * STRCAT(api_name, _iter_init)(struct RES_ROWS *);\
    struct RES_ROWS *STRCAT(api_name, _gen_rows)(MYSQL_RES *, int num_rows, int num_cols);\
    \
    \
    char **STRCAT(api_name, _iter_next)(ITER_STRUCT(API_NAME));\
    void STRCAT(api_name, _iter_reset)(ITER_STRUCT(API_NAME));\
    void STRCAT(api_name, _iter_free)(ITER_STRUCT(API_NAME));\

#endif
