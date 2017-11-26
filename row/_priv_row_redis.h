#ifndef _PRIV_ROW_REDIS_H
#define _PRIV_ROW_REDIS_H
#include "_priv_row.h"

struct REDIS_RES_ROWS_ITER {
    struct RES_ROWS_ITER super;
};

/**
 * "Constructor-like" function
 */
struct REDIS_RES_ROWS_ITER * redis_iter_init(struct RES_ROWS *);

struct RES_ROWS *redis_gen_rows(MYSQL_RES *, int num_rows, int num_cols);

char **redis_iter_next(struct REDIS_RES_ROWS_ITER *);

void redis_iter_reset(struct REDIS_RES_ROWS_ITER *);

void redis_iter_free(struct REDIS_RES_ROWS_ITER *);

#endif
