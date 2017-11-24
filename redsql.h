#ifndef REDSQL_H
#define REDSQL_H

#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>
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

/**
 * Read query result from cache if present, else execute SQL query and store
 * result in cache.
 * @param key the key name to store in cache
 * @param query the formatted query 
 * @param cache whether or not to cache the query
 *
 * @return RES_ROWS struct pointer containing query results, which can be
 * iterated using redsql_iterator
 */
RES_ROWS *redsql_read(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...);

/**
 * Write a formatted query to MySQL database. Takes a list of keys impacted
 * by the write, and evicts them from the redis cache.
 * 
 * @param evict_keys an array of keys to evict (passing in null will cause nothing to be evicted
 * @param evict_size length of evict_keys (pass in 0 if evict_keys is null)
 * @param query the formatted mysql query to execute
 *
 * @return number of 
 */
unsigned long redsql_write(struct redsql_conn *conn, const char *evict_keys[], size_t evict_size, const char *query, ...);

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
