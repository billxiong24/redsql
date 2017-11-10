#include <mysql/my_global.h>
#include <mysql/mysql.h>
#include <hiredis/async.h>
#include <hiredis/adapters/libevent.h>


struct query_info { 
    int num_rows;
    int affected_rows;

    char **result;
};

struct connection_info {
    MYSQL* mysql;
    redisAsyncContext *context;
};

struct connection_info *establish_conn(char *sql_host, char *sql_user, char *sql_pass, char *db, char *redis_host, unsigned int redis_port);

/**
 * Regular statements API
 */

struct query_info *read(const char *key, const char *query, ...);
struct query_info *write(const char *key, const char *query, ...);

/**
 * TODO Prepared statements API
 */
