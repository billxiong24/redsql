#ifndef SQL_API_H
#define SQL_API_H
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../types.h"
#include "../row/_priv_row.h"

/**
 * This function pointer is passed in to stream_read_query, and is called
 * on every row of the resultant query, like a function on a stream. 
 */
typedef void (*stream_func)(MYSQL_ROW, size_t num_rows, size_t num_cols);

/**
 * Given query format and variable argument list, generate parsed query
 *
 * @param query the formatted query to be parsed
 * @param args the variable argument list passed in from another function
 *
 * @return A parsed, executable SQL query
 */
char *gen_query(const char *query, va_list args);
/**
 * Any query to do with reading from database, i.e. SELECT, SHOW, DESCRIBE
 * returns the query result as an array of array of strings (char *)
 */
RES_ROWS_ITER *sql_read(MYSQL *mysql, const char *query, va_list args); 

/**
 * Any query to do with writing to database, i.e. INSERT, UPDATE, DELETE
 * returns number of affected rows
 */
uint32_t sql_write(MYSQL *mysql, const char *query, va_list args);

/**
 * Execute read query and stream results, takes in function to execute each row
 * as it is streamed through
 */
void sql_stream_read_query(MYSQL *mysql, const char *query, stream_func func, ...);

#endif
