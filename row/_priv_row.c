#include "_priv_row.h"
#include <stdio.h>
#include <string.h>

static void free_sql_rows(struct RES_ROWS *sql_rows) {
    if(sql_rows->type == REDIS_ROW_TYPE) {
        for (int i = 0; i < sql_rows->num_rows; i++) {
            ROW row = sql_rows->row_types.rows[i];
            //free each char *
            for(int j = 0; j < sql_rows->num_cols; j++) {
                free(row.fields[j]);
            }
            //free char **
            free(row.fields);
        }

        free(sql_rows->row_types.rows);
    }

    else {
        mysql_free_result(sql_rows->row_types.sql_rows);
        free(sql_rows);
    }

}

struct RES_ROWS *gen_rows(MYSQL_RES *result, ROW_TYPE type, int num_rows, int num_cols) {
    struct RES_ROWS *rows = malloc(sizeof(*rows));
    rows->type = type;
    rows->num_rows = num_rows;
    rows->num_cols = num_cols;

    if(rows->type == REDIS_ROW_TYPE) {
        rows->row_types.rows = malloc(sizeof(ROW) * (num_rows));
        
        for (int i = 0; i < num_rows; i++) {
            ROW row = rows->row_types.rows[i];
            row.fields = malloc(sizeof(char *) * num_cols);

             /*NOTE only client knows length of string to allocate, */
             /*so we let client allocate string lengths for each column result*/
            rows->row_types.rows[i] = row;
        }
    }
    else {
        //USING SAME MEMORY LOCATION, ALL DATA IS SHARED >:(
        rows->row_types.sql_rows = result;
    }
    return rows;
    /*struct RES_ROWS *sql_rows = malloc(sizeof(*sql_rows));*/ 
    /*sql_rows->num_rows = num_rows;*/
    /*sql_rows->num_cols = num_cols;*/

    /*sql_rows->rows = malloc(sizeof(ROW) * (num_rows));*/
    
    /*for (int i = 0; i < num_rows; i++) {*/
        /*ROW row = sql_rows->rows[i];*/
        /*row.fields = malloc(sizeof(char *) * num_cols);*/

        /**
         * NOTE only client knows length of string to allocate, 
         * so we let client allocate string lengths for each column result
         */
        /*sql_rows->rows[i] = row;*/
    /*}*/
    /*return sql_rows;*/
}

struct RES_ROWS_ITER *res_row_iterator(struct RES_ROWS *res_rows) {
    struct RES_ROWS_ITER *iter = malloc(sizeof(*iter));
    iter->res_rows = res_rows;
    iter->index = 0;
    if(res_rows->type == MYSQL_ROW_TYPE) {
        mysql_data_seek(res_rows->row_types.sql_rows, 0);
    }
    return iter;
}

char **res_row_next(struct RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->res_rows;

    if(rows->type == REDIS_ROW_TYPE) {
        if(iter->index >= rows->num_rows) {
            return NULL;
        }

        char **ret = rows->row_types.rows[iter->index].fields;
        iter->index++;
        return ret;
    }
    else {
        MYSQL_RES *res = rows->row_types.sql_rows;
        MYSQL_ROW row = mysql_fetch_row(res);
        iter->index++;
        return row;
    }
}

bool iter_has_next(struct RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->res_rows;
    return iter->index < rows->num_rows;
}

void reset_res_row(struct RES_ROWS_ITER *iter) {
    iter->index = 0;
    struct RES_ROWS *rows = iter->res_rows;

    if(rows->type == MYSQL_ROW_TYPE) {
        MYSQL_RES *res = rows->row_types.sql_rows;
        mysql_row_seek(res, 0);
    }
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
