#include "_priv_row.h"
#include <stdio.h>
#include <string.h>

/**
 * These define the common functions that are used by all subclasses.
 * Subclasses are able to override by using a different function in vtable.
 */
bool iter_has_next(struct RES_ROWS_ITER *iter) {
    struct RES_ROWS *rows = iter->res_rows;
    return iter->index < rows->num_rows;
}

size_t iter_num_rows(struct RES_ROWS_ITER *iter) {
    return iter->res_rows->num_rows;
}

size_t iter_num_cols(struct RES_ROWS_ITER *iter) {
    return iter->res_rows->num_cols;
}
