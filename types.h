#ifndef TYPES_H
#define TYPES_H

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



#endif
