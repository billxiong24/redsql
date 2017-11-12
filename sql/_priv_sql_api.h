#ifndef _PRIV_SQL_API_H
#define _PRIV_SQL_API_H


typedef struct {
    char **fields;
} ROW;

struct SQL_ROWS {
    ROW *rows;
    size_t num_rows;
    size_t num_cols;
};

#endif
