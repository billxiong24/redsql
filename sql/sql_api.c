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

static SQL_ROWS *gen_rows(int num_rows, int num_cols) {
    SQL_ROWS *sql_rows = malloc(sizeof(*sql_rows));

    sql_rows->num_rows = num_rows;
    sql_rows->num_cols = num_cols;

    sql_rows->rows = malloc(sizeof(ROW) * (num_rows));
    
    for (int i = 0; i < num_rows; i++) {
        ROW row = sql_rows->rows[i];
        row.fields = malloc(sizeof(char *) * num_cols);
        for(int j = 0; j < num_cols; j++) {
            //NOTE arbitrary number for now
            row.fields[j] = malloc(sizeof(char) * 10000);
        }
        sql_rows->rows[i] = row;
    }

    return sql_rows;
}

SQL_ROWS *read_query(MYSQL *mysql, char *query, ...) {

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

    SQL_ROWS *sql_rows = gen_rows(num_rows, num_cols);
    
    MYSQL_ROW row;
    register int j = 0;
    while(row = mysql_fetch_row(result)) {
        for (int i = 0; i < num_cols; i++) {
            strncpy(sql_rows->rows[j].fields[i], row[i], strlen(row[i]));
        }
        j++;
    }

    mysql_free_result(result);
    return sql_rows;
}

uint32_t write_query(MYSQL *mysql, char *query, ...) {
    return 0;
}

void stream_read_query(MYSQL *mysql, char *query, stream_func func, ...) {

}


uint32_t get_num_rows(SQL_ROWS *rows) {
    return rows->num_rows;
}

uint32_t get_num_cols(SQL_ROWS *rows) {
    return rows->num_cols;
}

char *get_row(SQL_ROWS *rows, uint32_t row, uint32_t col) {
    return rows->rows[row].fields[col];
    /*return rows->rows[index];*/
}

void free_sql_rows(SQL_ROWS *sql_rows) {
    for (int i = 0; i < sql_rows->num_rows; i++) {
        ROW row = sql_rows->rows[i];
        /*row.fields = malloc(sizeof(char *) * num_cols);*/
        for(int j = 0; j < sql_rows->num_cols; j++) {
            //NOTE arbitrary number for now
            free(row.fields[j]);
            /*row.fields[j] = malloc(sizeof(char) * 10000);*/
        }
        free(row.fields);
    }

    free(sql_rows->rows);
    free(sql_rows);
}
