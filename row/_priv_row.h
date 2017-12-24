/**
 * This header file provides a uniform iterator interface to iterate through query results,
 * whether they are cached in Redis or fetched from the database.
 */
#ifndef _PRIV_ROW_H
#define _PRIV_ROW_H


#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <mysql/mysql.h>

#define MYSQL_ROW_TYPE 0
#define REDIS_ROW_TYPE 1


struct RES_ROWS_ITER;

/**
 * return the next element in the resultant query struct (struct RES_ROWS *)
 * Entries in a column can be null, depending on the query result
 *
 * @return a char ** representing an array of strings. Each element in array comprises of a column entry. 
 */

extern char **res_row_iter_next(struct RES_ROWS_ITER *);

/**
 * Check if iterator has another element.
 * @return true if there is another element to be iterated over, else false
 */
extern bool res_row_iter_has_next(struct RES_ROWS_ITER *);

/**
 * reset iterator to the beginning element
 */
extern void res_row_iter_reset(struct RES_ROWS_ITER *);

/**
 * destructor for iterator for resultant query. Call this when done with
 * iterator, and are able to free up memory
 */
extern void res_row_iter_free(struct RES_ROWS_ITER *);

/**
 * @return the number of rows in resultant query held in the iterator
 */
extern size_t res_row_iter_rows(struct RES_ROWS_ITER *);

/**
 * @return the number of fields in the table of the resultant query
 */
extern size_t res_row_iter_cols(struct RES_ROWS_ITER *);

#endif
