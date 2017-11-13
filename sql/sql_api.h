#ifndef SQL_API_H
#define SQL_API_H
#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <inttypes.h>
#include <stdbool.h>


/**
 * Define an imcomplete struct type in order to encapsulate struct elements.
 * All functions in this file return a pointer to this struct- therefore the struct
 * does not have to be explicitly defined because pointers on the same machine are the same
 * size regardless of the pointed to value. The definition of struct RES_ROWS is in _priv_sql_api.h,
 * which should not be included by any end user.
 *
 */
typedef struct RES_ROWS RES_ROWS;

/**
 * This is defined as struct RES_ROWS_ITER in _priv_row.h, but makes it easier to typedef here
 */
typedef struct RES_ROWS_ITER RES_ROWS_ITER;


typedef void (*stream_func)(MYSQL_ROW, size_t num_rows, size_t num_cols);

/**
 * Any query to do with reading from database, i.e. SELECT, SHOW, DESCRIBE
 * returns the query result as an array of array of strings (char *)
 */
RES_ROWS *read_query(MYSQL *mysql, char *query, ...);

/**
 * Any query to do with writing to database, i.e. INSERT, UPDATE, DELETE
 * returns number of affected rows
 */
uint32_t write_query(MYSQL *mysql, char *query, ...);

/**
 * Execute read query and stream results, takes in function to execute each row
 * as it is streamed through
 */
void stream_read_query(MYSQL *mysql, char *query, stream_func func, ...);

/**
 * create an iterator for the resultant query
 */
struct RES_ROWS_ITER *sql_iter(struct RES_ROWS *);

/**
 * return the next element in the resultant query struct (struct RES_ROWS *)
 */
char **sql_iter_next(struct RES_ROWS_ITER *);

/**
 * return true if there is another element to be iterated over, else false
 */
bool sql_iter_has_next(struct RES_ROWS_ITER *);

/**
 * reset iterator to the beginning element
 */
void sql_iter_reset(struct RES_ROWS_ITER *);

/**
 * destructor for iterator for resultant query
 */
void sql_iter_free(struct RES_ROWS_ITER *);

/**
 * return the number of fields in resultant query's table.
 */
size_t sql_iter_num_cols(struct RES_ROWS_ITER *);

/**
 * debugging purposes
 */
void print_res(struct RES_ROWS *);

#endif
