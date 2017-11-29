#include "_priv_row_def.h"

struct RES_ROWS_ITER res_row_iter_init(struct RES_ROWS *rows, struct RES_ROW_VTABLE *vtable) {
    struct RES_ROWS_ITER iter;
    iter.res_rows = rows;
    iter.index = 0;
    iter.vtable = vtable;

    return iter;
}
