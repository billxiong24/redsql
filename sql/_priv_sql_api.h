#ifndef _PRIV_SQL_API_H
#define _PRIV_SQL_API_H
#include "_priv_row.h"

struct SQL_ROWS {
    ROW *rows;
    size_t num_rows;
    size_t num_cols;
};

#endif
