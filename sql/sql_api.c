#include <stdarg.h>
#include "sql_api.h"

static int err_check(MYSQL *mysql, int res) {
    if(res) {
      fprintf(stderr, " failed while closing the statement\n");
      fprintf(stderr, " %s\n", mysql_error(mysql));
      //TODO something with error handling
      return 1;
    }
    return 0;
}

static arr_list *gen_list(size_t rows, size_t cols) {
    arr_list *list = malloc(sizeof(arr_list));
    list->list = malloc(rows * sizeof(list));
    for(int i = 0; i < rows; i++) {
        list->list[i].string_arr = malloc(sizeof(char *) * cols);
    }
    return list;
}


arr_list *read_query(MYSQL *mysql, char *query, ...) {

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
    puts(buffer);

    if(err_check(mysql, mysql_query(mysql, buffer))) {
        return NULL;
    }

    MYSQL_RES *result = mysql_store_result(mysql);

    int num_rows = mysql_num_rows(result);
    int num_cols = mysql_num_fields(result);
    
    MYSQL_ROW row;
    register int i = 0;
    while(row = mysql_fetch_row(result)) {
        for(int j = 0; j < num_cols; j++) {
            if(row[j]) {
                puts(row[j]);
            }
            else {
                puts("nulld");
            }
        }
        i++; 
    }

    mysql_free_result(result);
    return NULL;
}

unsigned int write_query(MYSQL *mysql, char *query, ...) {
    return 0;
}

void stream_read_query(MYSQL *mysql, char *query, stream_func func, ...) {

}


