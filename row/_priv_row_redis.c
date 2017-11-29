#include "_priv_row_redis.h"
#include "_priv_row_def.h"
#include <stdio.h>

struct REDIS_RES_ROWS_ITER {
    struct RES_ROWS_ITER super;
};

struct RES_ROWS *redis_gen_rows(MYSQL_RES *result, int num_rows, int num_cols) {

    struct RES_ROWS *rows = malloc(sizeof(*rows));
    rows->type = REDIS_ROW_TYPE;
    rows->num_rows = num_rows;
    rows->num_cols = num_cols;

    rows->row_types.rows = malloc(sizeof(ROW) * (num_rows));
    
    for (int i = 0; i < num_rows; i++) {
        ROW row = rows->row_types.rows[i];
        row.fields = malloc(sizeof(char *) * num_cols);

         /*NOTE only client knows length of string to allocate, */
         /*so we let client allocate string lengths for each column result*/
        rows->row_types.rows[i] = row;
    }

    return rows;
}    

char **redis_iter_next(struct REDIS_RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->super.res_rows;
    if(iter->super.index >= rows->num_rows) {
        return NULL;
    }

    char **ret = rows->row_types.rows[iter->super.index].fields;
    iter->super.index++;
    return ret;
}

void redis_iter_reset(struct REDIS_RES_ROWS_ITER *iter) {
    iter->super.index = 0;
}

void redis_iter_free(struct REDIS_RES_ROWS_ITER *iter) {

    struct RES_ROWS *redis_rows = iter->super.res_rows;
    for (int i = 0; i < redis_rows->num_rows; i++) {
        ROW row = redis_rows->row_types.rows[i];
        //free each char *
        for(int j = 0; j < redis_rows->num_cols; j++) {
            free(row.fields[j]);
        }
        //free char **
        free(row.fields);
    }

    //free the ROW *
    free(redis_rows->row_types.rows);
    free(redis_rows);
    free(iter);
}

//put this at the end for now
struct RES_ROW_VTABLE redis_vtable = {
    &redis_gen_rows,
    (char **(*)(struct RES_ROWS_ITER *)) &redis_iter_next,
    &res_row_iter_has_next,
    (void (*)(struct RES_ROWS_ITER *)) &redis_iter_reset,
    (void (*)(struct RES_ROWS_ITER *)) &redis_iter_free,
    &res_row_iter_rows,
    &res_row_iter_cols,
};

struct REDIS_RES_ROWS_ITER * redis_iter_init(struct RES_ROWS * rows) {
    struct REDIS_RES_ROWS_ITER *iter = malloc(sizeof(*iter));
    iter->super = res_row_iter_init(rows, &redis_vtable);

    return iter;
}
