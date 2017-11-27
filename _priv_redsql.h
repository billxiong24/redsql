#ifndef _PRIV_REDSQL_H
#define _PRIV_REDSQL_H


struct redsql_conn {
    MYSQL* mysql;
    redisContext *context;
};

typedef struct redsql_conn redsql_conn;

#endif
