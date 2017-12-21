#include "_priv_row.h"
#include "_priv_row_def.h"
#include <stdio.h>
#include <string.h>

/**
 * These define the common functions that are used by all subclasses.
 * Subclasses are able to override by using a different function in vtable.
 */
bool res_row_iter_has_next(struct RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->res_rows;
    return iter->index < rows->num_rows;
}

size_t res_row_iter_rows(struct RES_ROWS_ITER *iter) {
    return iter->res_rows->num_rows;
}

size_t res_row_iter_cols(struct RES_ROWS_ITER *iter) {
    return iter->res_rows->num_cols;
}

/**
 * Vtable binding functions
 * These functions basically call a "subclasses" vtable function
 */

char **res_row_iter_next(struct RES_ROWS_ITER *iter){
    return iter->vtable->res_row_next(iter);
}

void res_row_iter_reset(struct RES_ROWS_ITER *iter){
    iter->vtable->reset_res_row(iter);
}

void res_row_iter_free(struct RES_ROWS_ITER *iter){
    iter->vtable->free_res_row_iter(iter);
}
