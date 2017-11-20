#include "redis_api.h"

//TODO add support for async calls

const char *NULL_DELIM = "(*@#$*!@(($#";

RES_ROWS *redis_read(redisContext *context, char *key) {
    redisReply *reply = redisCommand(context, "LRANGE %s 0 -1", key);

    if(reply->type == REDIS_REPLY_ARRAY) {
        int num_cols, num_rows;
        bool counted = false;
        
        for(int i = 0; i < reply->elements; i++) {
            if(strcmp(reply->element[i]->str, NULL_DELIM) == 0) {
                num_rows++;
                counted = true;
            }
            if(!counted)
                num_cols++;
        }

        RES_ROWS *res_rows = gen_rows(num_rows, num_cols);
        int counter = 0;
        for (int i = 0; i < num_rows; i++, counter++) {
            char *str = reply->element[counter]->str;

            for(int j = 0; j < num_cols; j++) {
                char *str = reply->element[counter]->str;
                strncpy(res_rows->rows[i].fields[j], str, strlen(str));
                counter++;
            }
        }
        
        freeReplyObject(reply);
        return res_rows;
    }

    freeReplyObject(reply);
    return NULL;
}

uint32_t redis_write(redisContext *context, char *key, RES_ROWS *rows, ...) {
    //TODO FIX THIS MEMORY LEAK
    RES_ROWS_ITER *iter = res_row_iterator(rows);
    void *r = redisCommand(context, "DEL %s", key);
    freeReplyObject(r);

    size_t num_cols = iter_num_cols(iter);

    register int count = 0;

    while(iter_has_next(iter)) {
        char **next = res_row_next(iter);
        for(int i = 0; i < num_cols; i++) {
            char *str = next[i];
            void *reply = redisCommand(context, "RPUSH %s %s", key, str);
            freeReplyObject(reply);
        }

        void *reply = redisCommand(context, "RPUSH %s %s", key, NULL_DELIM);
        freeReplyObject(reply);
        ++count;
    }
    return count;
}

struct RES_ROWS_ITER *redis_iter(struct RES_ROWS *rows) {
    return NULL;
}

char **redis_iter_next(struct RES_ROWS_ITER *iter) {
    return NULL;
}

bool redis_iter_has_next(struct RES_ROWS_ITER *iter) {
    return false;
}

void redis_iter_reset(struct RES_ROWS_ITER *iter) {

}

void redis_iter_free(struct RES_ROWS_ITER *iter) {

}

size_t redis_iter_num_cols(struct RES_ROWS_ITER *iter) {
    return 0;
}
