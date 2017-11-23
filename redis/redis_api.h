#ifndef REDIS_API_H 
#define REDIS_API_H

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../types.h"

//TODO add support for async calls

RES_ROWS *redis_read(redisContext *, const char *key);
uint32_t redis_write(redisContext *, const char *key, RES_ROWS *rows);

/**
 * Initialize iterator for resultant redis query
 */
struct RES_ROWS_ITER *redis_iter(struct RES_ROWS *);

/**
 * return the next element in the resultant query struct (struct RES_ROWS *)
 */
char **redis_iter_next(struct RES_ROWS_ITER *);

/**
 * return true if there is another element to be iterated over, else false
 */
bool redis_iter_has_next(struct RES_ROWS_ITER *);

/**
 * reset iterator to the beginning element
 */
void redis_iter_reset(struct RES_ROWS_ITER *);

/**
 * destructor for iterator for resultant query
 */
void redis_iter_free(struct RES_ROWS_ITER *);

/**
 * return the number of fields in resultant query's table.
 */
size_t redis_iter_num_cols(struct RES_ROWS_ITER *);

#endif
