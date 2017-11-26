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

/**
 * create an iterator for the resultant query
 */
//struct RES_ROWS_ITER *sql_iter(struct RES_ROWS *);

/**
 * return the next element in the resultant query struct (struct RES_ROWS *)
 */
//char **sql_iter_next(struct RES_ROWS_ITER *);

/**
 * return true if there is another element to be iterated over, else false
 */
//bool sql_iter_has_next(struct RES_ROWS_ITER *);

/**
 * reset iterator to the beginning element
 */
//void sql_iter_reset(struct RES_ROWS_ITER *);

/**
 * destructor for iterator for resultant query
 */
//void sql_iter_free(struct RES_ROWS_ITER *);

/**
 * return the number of fields in resultant query's table.
 */
//size_t sql_iter_num_cols(struct RES_ROWS_ITER *);

/**
 * debugging purposes
 */
//void print_res(struct RES_ROWS *);

#endif
