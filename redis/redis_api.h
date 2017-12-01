#ifndef REDIS_API_H 
#define REDIS_API_H

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../types.h"
#include "../row/_priv_row.h"

typedef struct REDIS_WRAP REDIS_WRAP;

/**
 * initializes REDIS_WRAP *
 * @param context the redisContext * to use
 *
 * @return a ptr to memory allocated for REDIS_WRAP * 
 */
REDIS_WRAP *redis_wrap_init(redisContext *context);

/**
 * Destructor for REDIS_WRAP *
 */
void redis_wrap_free(REDIS_WRAP *);

/**
 * @return underlying redisContext *
 */
redisContext *redis_wrap_get_context(REDIS_WRAP *);

/**
 *@return the error message held by REDIS_WRAP *ptr, else NULL
 */
char *redis_wrap_get_err(REDIS_WRAP *);

//TODO add support for async calls

/**
 * Reads query result from redis with specific key
 *
 * @param key the key to query from
 * @return RES_ROWS_ITER struct pointer containing query result info
 */
RES_ROWS_ITER *redis_read(REDIS_WRAP *, const char *key);

/**
 * Writes RES_ROWS_ITER struct pointer to redis cache
 *
 * @param key the key to store struct under
 * @param rows the struct pointer to store in redis
 *
 * @return number of rows added to redis cache
 */

uint32_t redis_write(REDIS_WRAP *, const char *key, RES_ROWS_ITER *rows);

#endif
