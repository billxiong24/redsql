#include "redsql.h"

const char *QUERY_KEY = "query:";

static void store_query(redisContext *context, const char *key, const char *query, va_list args_copy) {
    char *parsed_query = gen_query(query, args_copy);

    redisCommand(context, "SET %s%s %s", QUERY_KEY, key, parsed_query);

    free(parsed_query);
    va_end(args_copy);
}

struct redsql_conn *establish_conn(char *sql_host, char *sql_user, char *sql_pass, char *db, char *redis_host, unsigned int redis_port) {
    MYSQL *mysql = mysql_init(NULL);

    if(mysql_real_connect(mysql, sql_host, sql_user, sql_pass, db, 0, NULL, 0) == NULL) {
        fprintf(stderr, "MYSQL Connection failed.");
        exit(1);
    }
    redisContext *context = redisConnect(redis_host, redis_port);

    struct redsql_conn *conn = malloc(sizeof(*conn));
    conn->mysql = mysql;
    conn->context = context;

    return conn;
}

RES_ROWS *redsql_read(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...) {
    MYSQL *mysql = conn->mysql;
    redisContext *context = conn->context;

    va_list args, args_copy;
    va_start(args, cache);
    va_copy(args_copy, args);

    //check if key exists in redis
    redisReply *exists = redisCommand(context, "EXISTS %s", key);
    long long key_exists = 0L;
    if(exists->type == REDIS_REPLY_INTEGER) {
        key_exists = exists->integer;
    }

    freeReplyObject(exists);

    if(key_exists && cache) {
        puts("cache hit");
        return redis_read(context, key);
    }
    else {
        puts("cache miss");
        RES_ROWS *rows = sql_read(mysql, query, args);
        if(cache) {
            puts("writing to query result to cache");
            redis_write(context, key, rows);
            store_query(context, key, query, args_copy);
        }

        return rows;
    }
}

void redsql_write(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...) {

}

void free_redsql_conn(struct redsql_conn *conn) {
    mysql_close(conn->mysql);
    redisFree(conn->context);
    free(conn);
}
