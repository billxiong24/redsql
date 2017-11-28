#ifndef _PRIV_REDSQL_H
#define _PRIV_REDSQL_H

/**
 * This struct contains pointers to MYSQL and Redis connections, and encapsulates it from
 * the user. This struct should be hidden from the user. As such, this header file should not
 * be included in any client program.
 */

struct redsql_conn {
    MYSQL* mysql;
    redisContext *context;
};

typedef struct redsql_conn redsql_conn;

#endif
