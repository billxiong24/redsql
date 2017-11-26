#ifndef _PRIV_ROW_SQL_H
#define _PRIV_ROW_SQL_H
#include "_priv_row.h"

struct SQL_RES_ROWS_ITER {
    struct RES_ROWS_ITER super;
};

/**
 * "Constructor-like" function
 */
struct SQL_RES_ROWS_ITER * sql_iter_init(struct RES_ROWS *);

struct RES_ROWS *sql_gen_rows(MYSQL_RES *, int num_rows, int num_cols);

char **sql_iter_next(struct SQL_RES_ROWS_ITER *);

void sql_iter_reset(struct SQL_RES_ROWS_ITER *);

void sql_iter_free(struct SQL_RES_ROWS_ITER *);

#endif
