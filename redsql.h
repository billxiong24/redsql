#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
#include "row/_priv_row.h"
#include "types.h"
#include "redis/redis_api.h"
#include "sql/sql_api.h"
#include <stdbool.h>


//NOTE keep this hidden from user
struct redsql_conn {
    MYSQL* mysql;
    redisContext *context;
};

struct redsql_conn *establish_conn(char *sql_host, char *sql_user, char *sql_pass, char *db, char *redis_host, unsigned int redis_port);

/**
 * Regular statements API
 */

RES_ROWS *read(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...);
void write(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...);

void free_redsql_conn(struct redsql_conn *conn);

/**
 * TODO Prepared statements API
 */
