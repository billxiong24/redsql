#include "redsql.h"

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

RES_ROWS *read(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...) {
    MYSQL *mysql = conn->mysql;
    redisContext *context = conn->context;
    RES_ROWS *rows;

    //check if key exists in redis
    redisReply *exists = redisCommand(context, "EXISTS %s", key);
    long long key_exists = 0L;
    if(exists->type == REDIS_REPLY_INTEGER) {
        key_exists = exists->integer;
    }
    freeReplyObject(exists);

    if(key_exists && cache) {
        return redis_read(context, key);
    }
    else {
        //TODO pass in va_args here
        rows = sql_read(mysql, query);
        if(cache) {
            redis_write(context, key, rows);
        }
        return rows;
    }
}

void write(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...) {

}

void free_redsql_conn(struct redsql_conn *conn) {

}
