#include "redis_api.h"

//TODO add support for async calls

RES_ROWS *redis_read(redisContext *context, char *query, ...) {
    return NULL;
}

uint32_t redis_write(redisContext *context, char *key, RES_ROWS *rows, ...) {
    tpl_node *tn;
    RES_ROWS_ITER *iter = res_row_iterator(rows);

    size_t num_cols = iter_num_cols(iter);

    char *str;
    tn = tpl_map("A(A(s))", &str);

    while(iter_has_next(iter)) {
        char **next = res_row_next(iter);
        for(int i = 0; i < num_cols; i++) {
            str = next[i];
            if(str) {
                tpl_pack(tn, 2);
            }
        }
        tpl_pack(tn, 1);
    }
    char *buff;
    size_t size;
    tpl_dump(tn, TPL_GETSIZE, &size);
    tpl_dump(tn, TPL_MEM, &buff, &size);

    return 0;
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
