#include "redis_api.h"
#include "../row/_priv_row_redis.h"
#include "../row/_priv_row_def.h"
//TODO add support for async calls

RES_ROWS_ITER *redis_read(redisContext *context, const char *key) {
    redisReply *reply = redisCommand(context, "LRANGE %s 0 -1", key);

    if(reply->type != REDIS_REPLY_ARRAY) {
        freeReplyObject(reply);
        return NULL;
    }

    int num_cols = 0;
    int num_rows = 0;
    bool counted = false;

    //figure out number of rows and columns
    for(int i = 0; i < reply->elements; i++) {
        if(strcmp(reply->element[i]->str, NULL_DELIM) == 0) {
            num_rows++;
            counted = true;
        }
        if(!counted)
            num_cols++;
    }

    RES_ROWS *res_rows = redis_gen_rows(NULL, num_rows, num_cols);
    int counter = 0;
    for (int i = 0; i < num_rows; i++, counter++) {
        for(int j = 0; j < num_cols; j++) {
            char *str = reply->element[counter]->str;
            //XXX Always check that the string is not null!! stupid bug
            if(!str) {
                counter++;
                continue;
            }
            else if(strcmp(str, NULL_SYM) == 0) {
                res_rows->row_types.rows[i].fields[j] = NULL;
            }
            else {
                size_t len = strlen(str);
                res_rows->row_types.rows[i].fields[j] = malloc(sizeof(char) * len + 1);
                strncpy(res_rows->row_types.rows[i].fields[j], str, len);
                res_rows->row_types.rows[i].fields[j][len] = '\0';

            }

            counter++;
        }
    }

    freeReplyObject(reply);

    struct REDIS_RES_ROWS_ITER * iter = redis_iter_init(res_rows);
    return (RES_ROWS_ITER *) iter;
}

uint32_t redis_write(redisContext *context, const char *key, RES_ROWS_ITER *iter) {
    if(iter->res_rows->type != MYSQL_ROW_TYPE) {
        //TODO some eror handling here
        return -1;
    }
    void *r = redisCommand(context, "DEL %s", key);
    freeReplyObject(r);

    size_t num_cols = RES_ROW_ITER_FUNC(iter, iter_num_cols);

    register int count = 0;

    while(RES_ROW_ITER_FUNC(iter, iter_has_next)) {
        char **next = RES_ROW_ITER_FUNC(iter, res_row_next);
        for(int i = 0; i < num_cols; i++) {
            char *str = next[i];
            if(!str) {
                str = NULL_SYM;
            }
            void *reply = redisCommand(context, "RPUSH %s %s", key, str);
            freeReplyObject(reply);
        }

        void *reply = redisCommand(context, "RPUSH %s %s", key, NULL_DELIM);
        freeReplyObject(reply);
        ++count;
    }

    //make sure to not free this iter, bc it will be returned, just using it
    //this saves memory, since we do not duplicate iterator, just use it

    RES_ROW_ITER_FUNC(iter, reset_res_row);
    return count;
}
