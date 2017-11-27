#include "row/_priv_row.h"
#include "redsql.h"
#include "_priv_redsql.h"

#define QUERY_KEY "query:"

static void store_query(redisContext *context, const char *key, const char *query, va_list args_copy) {
    char *parsed_query = gen_query(query, args_copy);

    void *rep = redisCommand(context, "SET %s%s %s", QUERY_KEY, key, parsed_query);

    freeReplyObject(rep);
    free(parsed_query);
    va_end(args_copy);
}

static unsigned long evict_from_cache(redisContext *context, const char *keys[], size_t size) {
    void *mult = redisCommand(context, "MULTI");

    for (int i = 0; i < size; i++) {
       void *del_key = redisCommand(context, "DEL %s", keys[i]);
       void *del_quer = redisCommand(context, "DEL %s%s", QUERY_KEY, keys[i]);
       freeReplyObject(del_key);
       freeReplyObject(del_quer);
    }

    freeReplyObject(mult);
    redisReply *reply = redisCommand(context, "EXEC");

    unsigned long count = 0L;
    if(reply->type == REDIS_REPLY_ARRAY) {
        for (int i = 0; i < reply->elements; i++) {
            redisReply *temp = reply->element[i];
            //if reply == 1, element was removed, else no element was removed
            if(temp->type == REDIS_REPLY_INTEGER && temp->integer > 0) {
                ++count;
            }
        }
    }
    freeReplyObject(reply);
    /**
     * return count / 2 because deleting 2 keys for every query; we want to return 
     * number of queries deleted from redis
     */
    return count / 2;
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

char *redsql_get_stored_query(struct redsql_conn *conn, const char *key) {
    redisContext *context = conn->context;
    redisReply *reply = redisCommand(context, "GET %s", key);

    char *str = NULL;
    if(reply->type == REDIS_REPLY_STRING) {
        str = reply->str;
    }

    freeReplyObject(reply);
    return str;
}

bool redsql_in_cache(struct redsql_conn *conn, const char *key) {
    redisContext *context = conn->context;
    redisReply *reply = redisCommand(context, "EXISTS %s", key);

    bool res = false;

    if(reply->type == REDIS_REPLY_INTEGER) {
        res = reply->integer == 1 ? true : false;
    }

    freeReplyObject(reply);
    return res;
}

bool redsql_evict(struct redsql_conn *conn, const char *key) {
    redisContext *context = conn->context;

    const char *evict[1];
    evict[0] = key;

    unsigned long res = evict_from_cache(context, evict, 1);
    return res > 0;
}

RES_ROWS_ITER *redsql_read(struct redsql_conn *conn, const char *key, const char *query, bool cache, ...) {
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
       
        RES_ROWS_ITER *iter = sql_read(mysql, query, args);

        if(cache) {
            puts("writing to query result to cache");
            redis_write(context, key, iter);
            store_query(context, key, query, args_copy);
        }
        
        res_row_iter_reset(iter);
        return iter;
    }
}

unsigned long redsql_write(struct redsql_conn *conn, const char *evict_keys[], size_t evict_size, const char *query, ...) {
    MYSQL *mysql = conn->mysql;
    redisContext *context = conn->context;

    va_list args;
    va_start(args, query);

    sql_write(mysql, query, args);

    //evict list of keys
    if(evict_keys) {
        return evict_from_cache(context, evict_keys, evict_size);
    }

    return 0L;
}

void free_redsql_conn(struct redsql_conn *conn) {
    mysql_close(conn->mysql);
    redisFree(conn->context);
    free(conn);
}
