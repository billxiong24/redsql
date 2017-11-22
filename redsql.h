#ifndef REDSQL_H
#define REDSQL_H

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

RES_ROWS *redsql_read(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...);

void redsql_write(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...);

/**
 * Returns the query associated with a specific key
 * @param key the key to retrieve query from
 *
 * @return string query
 */
char *redsql_get_stored_query(struct redsql_conn *, const char *key);

/**
 * Checks if given key is in redis
 * @param key the key to retrieve query from 
 *
 * @return true if in redis, else false
 */
bool redsql_in_cache(struct redsql_conn *, const char *key);


/**
 * Evict given key from cache, including query results and stored query
 * @param key the key to evict
 *
 * @return true if key was evicted, else false
 */
bool redsql_evict(struct redsql_conn *, const char *key);

/**
 * Destructor for redsql_conn struct
 */
void free_redsql_conn(struct redsql_conn *conn);

/**
 * TODO Prepared statements API
 */

#endif  
