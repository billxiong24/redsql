#include <stdarg.h>
#include "sql_api.h"
#include "_priv_sql_api.h"

static int err_check(MYSQL *mysql, int res) {
    if(res) {
      fprintf(stderr, " failed while closing the statement\n");
      fprintf(stderr, " %s\n", mysql_error(mysql));
      //TODO something with error handling
      return 1;
    }
    return 0;
}

RES_ROWS *read_query(MYSQL *mysql, char *query, ...) {

    va_list args, args_reuse;
    va_start(args, query);
    va_copy(args_reuse, args);

    //write 0 characters to null pointer to determine size of characters should have written
    int size = 1 + vsnprintf(NULL, 0, query, args);
    char buffer[size];
    //we now know the appropriate suze to allocate to avoid buffer overflow
    vsnprintf(buffer, size, query, args_reuse);
    va_end(args);
    va_end(args_reuse);

    if(err_check(mysql, mysql_query(mysql, buffer))) {
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(mysql);

    int num_rows = mysql_num_rows(result);
    int num_cols = mysql_num_fields(result);

    RES_ROWS *sql_rows = gen_rows(num_rows, num_cols);
    
    MYSQL_ROW row;
    register int j = 0;
    while(row = mysql_fetch_row(result)) {
        for (int i = 0; i < num_cols; i++) {
            strncpy(sql_rows->rows[j].fields[i], row[i], strlen(row[i]));
        }
        j++;
    }


    mysql_free_result(result);
    /*for(ijt i = 0; i < num_rows; i++) {*/
        /*char ** str= sql_rows->rows[i].fields;*/
        /*for(int j = 0; j < num_cols; j++) {*/
            /*puts(str[j]);*/
        /*}*/
    /*}*/
    return sql_rows;
}

uint32_t write_query(MYSQL *mysql, char *query, ...) {
    return 0;
}

void stream_read_query(MYSQL *mysql, char *query, stream_func func, ...) {

}

struct RES_ROWS_ITER *sql_iter(struct RES_ROWS *rows) {
    return res_row_iterator(rows);
}

char **sql_iter_next(struct RES_ROWS_ITER *iter) {
    return res_row_next(iter);
}

bool sql_iter_has_next(struct RES_ROWS_ITER *iter) {
    return iter_has_next(iter);
}

void sql_iter_reset(struct RES_ROWS_ITER *iter) {
    reset_res_row(iter);
}

size_t sql_iter_num_cols(struct RES_ROWS_ITER *iter) {
    return iter_num_cols(iter);
}

void sql_iter_free(struct RES_ROWS_ITER *iter) {
    free_res_row_iter(iter);
}


void print_res(struct RES_ROWS *sql_rows) {
    for(int i = 0; i < sql_rows->num_rows; i++) {
        char ** str= sql_rows->rows[i].fields;
        for(int j = 0; j < sql_rows->num_cols; j++) {
            puts(str[j]);
        }
    }

}
