#include "_priv_row_sql.h"
#include "_priv_row_def.h"
#include <stdio.h>

struct SQL_RES_ROWS_ITER {
    struct RES_ROWS_ITER super;
};

#define gen_header(name) (\
        struct RES_ROWS;\
        struct name_RES_ROWS_ITER\
        )
struct RES_ROWS *sql_gen_rows(MYSQL_RES *result, int num_rows, int num_cols) {
    struct RES_ROWS *rows = malloc(sizeof(*rows));
    rows->type = MYSQL_ROW_TYPE;
    rows->num_rows = num_rows;
    rows->num_cols = num_cols;
    rows->row_types.sql_rows = result;

    return rows;
}

char **sql_iter_next(struct SQL_RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->super.res_rows;

    MYSQL_RES *res = rows->row_types.sql_rows;
    MYSQL_ROW row = mysql_fetch_row(res);
    iter->super.index++;

    return (char **) row;
}

void sql_iter_reset(struct SQL_RES_ROWS_ITER *iter) {
    iter->super.index = 0;
    struct RES_ROWS *rows = iter->super.res_rows;
    MYSQL_RES *res = rows->row_types.sql_rows;
    mysql_data_seek(res, 0);
}

void sql_iter_free(struct SQL_RES_ROWS_ITER *iter) {
    struct RES_ROWS *sql_rows = iter->super.res_rows;
    mysql_free_result(sql_rows->row_types.sql_rows); 
    free(sql_rows);
    free(iter);
}

struct RES_ROW_VTABLE sql_vtable = {

    &sql_gen_rows,
    (char **(*)(struct RES_ROWS_ITER *)) &sql_iter_next,
    &res_row_iter_has_next,
    (void (*)(struct RES_ROWS_ITER *)) &sql_iter_reset,
    (void (*)(struct RES_ROWS_ITER *)) &sql_iter_free,
    &res_row_iter_rows,
    &res_row_iter_cols,
};

struct SQL_RES_ROWS_ITER *sql_iter_init(struct RES_ROWS *rows) {
    struct SQL_RES_ROWS_ITER *iter = malloc(sizeof(*iter));
    iter->super.res_rows = rows;
    iter->super.index = 0;
    iter->super.vtable = &sql_vtable;

    return iter;
}

