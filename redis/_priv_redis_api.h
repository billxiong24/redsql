#ifndef __PRIV_REDIS_API_H
#define __PRIV_REDIS_API_H

struct REDIS_WRAP {
    redisContext *context;
    char *err;
};

#endif
