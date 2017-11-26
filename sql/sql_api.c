#include <stdarg.h>
#include "sql_api.h"
#include "_priv_sql_api.h"
#include "../row/_priv_row_sql.h"

static int err_check(MYSQL *mysql, int res) {
    if(res) {
      fprintf(stderr, " failed while closing the statement\n");
      fprintf(stderr, " %s\n", mysql_error(mysql));
      //TODO something with error handling
      return 1;
    }
    return 0;
}

static MYSQL_RES *exec_query(MYSQL *mysql, const char *query, va_list args) {

    char *buffer = gen_query(query, args);

    if(err_check(mysql, mysql_query(mysql, buffer))) {
        return NULL;
    }

    free(buffer);
    return mysql_store_result(mysql);
}

char *gen_query(const char *query, va_list args) {
    va_list args_reuse;
    va_copy(args_reuse, args);

    int size = 1 + vsnprintf(NULL, 0, query, args);
    char *buffer = malloc(sizeof(char) * size);
    //we now know the appropriate suze to allocate to avoid buffer overflow
    vsnprintf(buffer, size, query, args_reuse);
    va_end(args);
    va_end(args_reuse);
    return buffer;
}

RES_ROWS_ITER *sql_read(MYSQL *mysql, const char *query, va_list args) {

    MYSQL_RES *result = exec_query(mysql, query, args);

    int num_rows = mysql_num_rows(result);
    int num_cols = mysql_num_fields(result);

    RES_ROWS *sql_rows = sql_gen_rows(result, num_rows, num_cols);
    RES_ROWS_ITER *iter = (RES_ROWS_ITER *) sql_iter_init(sql_rows);
    return iter;
}

uint32_t sql_write(MYSQL *mysql, const char *query, va_list args) {

    MYSQL_RES *result = exec_query(mysql, query, args);
    mysql_free_result(result);
    return mysql_affected_rows(mysql);
}

void sql_stream_read_query(MYSQL *mysql, const char *query, stream_func func, ...) {
    va_list args;
    va_start(args, func);

    MYSQL_RES *result = exec_query(mysql, query, args);

    int num_rows = mysql_num_rows(result);
    int num_cols = mysql_num_fields(result);

    MYSQL_ROW row;

    while(row = mysql_fetch_row(result)) {
        func(row, num_rows, num_cols);
    }
    mysql_free_result(result);
}
