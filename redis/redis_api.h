#ifndef REDIS_API_H 
#define REDIS_API_H

#include <hiredis/hiredis.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "../types.h"
#include "../row/_priv_row.h"

//TODO add support for async calls

RES_ROWS_ITER *redis_read(redisContext *, const char *key);
uint32_t redis_write(redisContext *, const char *key, RES_ROWS_ITER *rows);

#endif
