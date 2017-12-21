/**
 * This header file is defined a means of encapsulation from the client/end user.
 * This file should not be included in any other module besides implementation modules.
 * Said implementation modules should be the modules included by the client.
 */
#ifndef _PRIV_ROW_H
#define _PRIV_ROW_H

#define MYSQL_ROW_TYPE 0
#define REDIS_ROW_TYPE 1

#define NULL_DELIM "(*@#$(*$W%9747(@*#$)*@#$(*$%&#$%*&__!@))#$*!@(($#"
#define NULL_SYM "((NULL))"

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <mysql/mysql.h>

struct RES_ROWS_ITER;

/**
 * return the next element in the resultant query struct (struct RES_ROWS *)
 * Entries in a column can be null, depending on the query result
 *
 * @return a char ** representing an array of strings. Each element in array comprises of a column entry. 
 */
char **res_row_iter_next(struct RES_ROWS_ITER *);

/**
 * Check if iterator has another element.
 * @return true if there is another element to be iterated over, else false
 */
bool res_row_iter_has_next(struct RES_ROWS_ITER *);

/**
 * reset iterator to the beginning element
 */
void res_row_iter_reset(struct RES_ROWS_ITER *);

/**
 * destructor for iterator for resultant query. Call this when done with
 * iterator, and are able to free up memory
 */
void res_row_iter_free(struct RES_ROWS_ITER *);

/**
 * @return the number of rows in resultant query held in the iterator
 */
size_t res_row_iter_rows(struct RES_ROWS_ITER *);

/**
 * @return the number of fields in the table of the resultant query
 */
size_t res_row_iter_cols(struct RES_ROWS_ITER *);

#endif
