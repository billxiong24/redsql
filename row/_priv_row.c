#include "_priv_row.h"
#include <stdio.h>

static void free_sql_rows(struct RES_ROWS *sql_rows) {
    for (int i = 0; i < sql_rows->num_rows; i++) {
        ROW row = sql_rows->rows[i];
        //free each char *
        for(int j = 0; j < sql_rows->num_cols; j++) {
            free(row.fields[j]);
        }
        //free char **
        free(row.fields);
    }

    //free ROW *
    free(sql_rows->rows);

    //finally free RES_ROWS *
    free(sql_rows);
}

struct RES_ROWS *gen_rows(int num_rows, int num_cols) {
    struct RES_ROWS *sql_rows = malloc(sizeof(*sql_rows));

    sql_rows->num_rows = num_rows;
    sql_rows->num_cols = num_cols;

    sql_rows->rows = malloc(sizeof(ROW) * (num_rows));
    
    for (int i = 0; i < num_rows; i++) {
        ROW row = sql_rows->rows[i];
        row.fields = malloc(sizeof(char *) * num_cols);
        for(int j = 0; j < num_cols; j++) {
            //NOTE arbitrary number for now
            row.fields[j] = malloc(sizeof(char) * 3000);
        }
        sql_rows->rows[i] = row;
    }

    return sql_rows;
}

struct RES_ROWS_ITER *res_row_iterator(struct RES_ROWS *res_rows) {
    struct RES_ROWS_ITER *iter = malloc(sizeof(*iter));
    iter->res_rows = res_rows;
    iter->index = 0;
        
    return iter;
}

char **res_row_next(struct RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->res_rows;
    
    if(iter->index >= rows->num_rows) {
        return NULL;
    }
    char **ret = rows->rows[iter->index].fields;

    iter->index++;

    return ret;
}

bool iter_has_next(struct RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->res_rows;
    return iter->index < rows->num_rows;
}

void reset_res_row(struct RES_ROWS_ITER *iter) {
    iter->index = 0;
}

void free_res_row_iter(struct RES_ROWS_ITER *iter) {
    free_sql_rows(iter->res_rows);
    free(iter);
}

size_t iter_num_rows(struct RES_ROWS_ITER *iter) {
    return iter->res_rows->num_rows;
}

size_t iter_num_cols(struct RES_ROWS_ITER *iter) {
    return iter->res_rows->num_cols;
}
