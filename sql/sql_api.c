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

static int err_null_check(MYSQL_WRAP *wrap , const char *query, va_list args) {
    int err = 0;

    if(!query) {
        err = ERR_NULL_STR;
    }
    if(!wrap) {
        err = ERR_NULL_WRAP;
        return err;
    }
    if(!wrap->mysql) {
        err = ERR_NULL_CONN;
    }
    if(!args) {
        err = ERR_NULL_ARGS;
    }

    if(err != 0) {
        wrap->err = ERR_NULL_PTR;
    }

    return err;
}

static MYSQL_RES *exec_query(MYSQL_WRAP *wrap, const char *query, va_list args) {

    MYSQL *mysql = wrap->mysql;
    char *buffer = gen_query(query, args);

    if(err_check(mysql, mysql_query(mysql, buffer))) {
        wrap->err = (char *) mysql_error(mysql);
        return NULL;
    }

    free(buffer);
    return mysql_store_result(mysql);
}

MYSQL_WRAP *mysql_wrap_init(MYSQL *mysql) {
    MYSQL_WRAP *wrap = malloc(sizeof(*wrap));
    wrap->mysql = mysql;
    wrap->err = NULL;
    return wrap;
}

char *mysql_wrap_get_err(MYSQL_WRAP *wrap) {
    return wrap->err;
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

RES_ROWS_ITER *sql_read(MYSQL_WRAP *wrap, const char *query, va_list args) {
    if(err_null_check(wrap, query, args)) {
        return NULL;
    }

    MYSQL_RES *result = exec_query(wrap, query, args);
    if(!result) {
        //TODO find out if we need to free mem here
        return NULL;
    }

    int num_rows = mysql_num_rows(result);
    int num_cols = mysql_num_fields(result);

    RES_ROWS *sql_rows = sql_gen_rows(result, num_rows, num_cols);
    RES_ROWS_ITER *iter = (RES_ROWS_ITER *) sql_iter_init(sql_rows);

    return iter;
}

int32_t sql_write(MYSQL_WRAP *wrap, const char *query, va_list args) {
    int check; 
    if((check = err_null_check(wrap, query, args)) < 0) {
        return check;
    }


    MYSQL *mysql = wrap->mysql;

    //result could be null, but that does not mean write query failed
    MYSQL_RES *result = exec_query(wrap, query, args);

    mysql_free_result(result);
    return mysql_affected_rows(mysql);
}

void sql_stream_read_query(MYSQL_WRAP *wrap, const char *query, stream_func func, ...) {
    //TODO "throw" some exception here
    if(!query) {
        return;
    }

    va_list args;
    va_start(args, func);

    MYSQL_RES *result = exec_query(wrap, query, args);

    if(!result) {
        return;
    }

    int num_rows = mysql_num_rows(result);
    int num_cols = mysql_num_fields(result);

    MYSQL_ROW row;

    while((row = mysql_fetch_row(result))) {
        func(row, num_rows, num_cols);
    }
    mysql_free_result(result);
}

void mysql_wrap_free(MYSQL_WRAP *wrap) {
    mysql_close(wrap->mysql);
    free(wrap);
}
