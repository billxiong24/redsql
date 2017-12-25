#ifndef __PRIV_REDIS_API_H
#define __PRIV_REDIS_API_H

/**
 * If consumer of this library, do not include this file!
 * This file is for internal use in conjunction with redis_api.h- Refer to redis_api.h for public API
 */

struct REDIS_WRAP {
    redisContext *context;
    char *err;
};

#endif
