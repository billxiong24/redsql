#include "redis_api.h"

//TODO add support for async calls

const char *NULL_DELIM = "(*@#$(*$W%9747(@*#$)*@#$(*$%&#$%*&__!@))#$*!@(($#";

RES_ROWS *redis_read(redisContext *context, const char *key) {
    redisReply *reply = redisCommand(context, "LRANGE %s 0 -1", key);

    if(reply->type == REDIS_REPLY_ARRAY) {
        int num_cols = 0;
        int num_rows = 0;
        bool counted = false;
        
        for(int i = 0; i < reply->elements; i++) {
            if(strcmp(reply->element[i]->str, NULL_DELIM) == 0) {
                num_rows++;
                counted = true;
            }
            if(!counted)
                num_cols++;
        }

        RES_ROWS *res_rows = gen_rows(NULL, REDIS_ROW_TYPE, num_rows, num_cols);
        int counter = 0;
        for (int i = 0; i < num_rows; i++, counter++) {
            for(int j = 0; j < num_cols; j++) {
                char *str = reply->element[counter]->str;
                //XXX Always check that the string is not null!! stupid bug
                if(str) {
                    size_t len = strlen(str);
                    res_rows->row_types.rows[i].fields[j] = malloc(sizeof(char) * len + 1);
                    strncpy(res_rows->row_types.rows[i].fields[j], str, len);
                    res_rows->row_types.rows[i].fields[j][len] = '\0';
                }
                counter++;
            }
        }
        
        freeReplyObject(reply);
        return res_rows;
    }

    freeReplyObject(reply);
    return NULL;
}

uint32_t redis_write(redisContext *context, const char *key, RES_ROWS *rows) {
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

    free(iter);

    return count;
}

struct RES_ROWS_ITER *redis_iter(struct RES_ROWS *rows) {
    return res_row_iterator(rows);
}

char **redis_iter_next(struct RES_ROWS_ITER *iter) {
    return res_row_next(iter);
}

bool redis_iter_has_next(struct RES_ROWS_ITER *iter) {
    return iter_has_next(iter);
}

void redis_iter_reset(struct RES_ROWS_ITER *iter) {
    reset_res_row(iter);
}

void redis_iter_free(struct RES_ROWS_ITER *iter) {
    free_res_row_iter(iter);
}

size_t redis_iter_num_cols(struct RES_ROWS_ITER *iter) {
    return iter_num_cols(iter);
}
