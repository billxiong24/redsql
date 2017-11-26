/**
 * This header file is defined a means of encapsulation from the client/end user.
 * This file should not be included in any other module besides implementation modules.
 * Said implementation modules should be the modules included by the client.
 */
#ifndef _PRIV_ROW_H
#define _PRIV_ROW_H

#define MYSQL_ROW_TYPE 0
#define REDIS_ROW_TYPE 1

#define ROW_TYPE unsigned char

#define NULL_DELIM "(*@#$(*$W%9747(@*#$)*@#$(*$%&#$%*&__!@))#$*!@(($#"
#define NULL_SYM "((NULL))"

#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <mysql.h>

/**
 * Simple encapsulation of a char ** for clarity
 * In this case, char ** is an array of strings
 */
typedef struct {
    char **fields;
} ROW;

/**
 * This struct stores resultant query information
 */ 
struct RES_ROWS {
    /**
     * defines row type- ROW is for redis results,
     * MYSQL_RES Is for SQL query results
     */
    union {
        ROW *rows;
        MYSQL_RES *sql_rows;
    } row_types;
    /**
     * Tag to specify current variable used in the row_types union
     */
    ROW_TYPE type;


    /**
     * Number of rows in the resultant query, and number of columns in the table
     */
    size_t num_rows;
    size_t num_cols;
};

/**
 * iterator for query result
 */
struct RES_ROWS_ITER {
    /**
     * Pointer to VTABLE for subclasses to access and override with 
     * custom functions
     */
    struct RES_ROW_VTABLE *vtable;
    /**
     * resultant query store
     */
    struct RES_ROWS *res_rows;

    /**
     * the current index that the iterator is on
     */
    uint32_t index;
};

/**
 * Virtual function table for struct RES_ROWS_ITER,
 * allows subclasses to inherit methods and implement "abstract"  methods
 */

struct RES_ROW_VTABLE {
    /**
     * initialize struct RES_ROWS * for use 
     */
    struct RES_ROWS * (*gen_rows)(MYSQL_RES *, int num_rows, int num_cols);

    /**
     * return the next element in the resultant query struct (struct RES_ROWS *)
     */
    char **(*res_row_next)(struct RES_ROWS_ITER *);

    /**
     * return true if there is another element to be iterated over, else false
     */
    bool (*iter_has_next)(struct RES_ROWS_ITER *);

    /**
     * reset iterator to the beginning element
     */
    void (*reset_res_row)(struct RES_ROWS_ITER *);

    /**
     * destructor for iterator for resultant query
     */
    void (*free_res_row_iter)(struct RES_ROWS_ITER *);

    /**
     * returns the number of rows in resultant query
     */
    size_t (*iter_num_rows)(struct RES_ROWS_ITER *);

    /**
     * returns the number of fields in the table of the resultant query
     */
    size_t (*iter_num_cols)(struct RES_ROWS_ITER *);
};

//XXX very unsafe, find better way to do this
#define RES_ROW_ITER_FUNC(res_row_iter, func) (\
        res_row_iter->vtable->func(res_row_iter)\
        )

/**
 * initialize struct RES_ROWS * for use 
 */
struct RES_ROWS *gen_rows(MYSQL_RES *, ROW_TYPE type, int num_rows, int num_cols);

/**
 * initialize iterator for struct RES_ROWS *
 */
struct RES_ROWS_ITER *res_row_iterator(struct RES_ROWS *);

/**
 * return the next element in the resultant query struct (struct RES_ROWS *)
 */
char **res_row_next(struct RES_ROWS_ITER *);

/**
 * return true if there is another element to be iterated over, else false
 */
bool iter_has_next(struct RES_ROWS_ITER *);

/**
 * reset iterator to the beginning element
 */
void reset_res_row(struct RES_ROWS_ITER *);

/**
 * destructor for iterator for resultant query
 */
void free_res_row_iter(struct RES_ROWS_ITER *);

/**
 * returns the number of rows in resultant query
 */
size_t iter_num_rows(struct RES_ROWS_ITER *);

/**
 * returns the number of fields in the table of the resultant query
 */
size_t iter_num_cols(struct RES_ROWS_ITER *);

#endif
