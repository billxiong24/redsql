#include "../../CuTest.h"
#include <hiredis/hiredis.h>
#include <stdio.h>
#include "../redis_api.h"
#include "../../sql/sql_api.h"
#include <stdarg.h>

//TODO maybe use mock objects thru dependency injection

/**
 * For now, just use sql_api.c to write query results into Redis.
 * This is not ideal, since sql_api.c should be mocked out.
 * As of now, make sure sql_api passes all tests before running these
 * tests.
 */

REDIS_WRAP *redis_wrap;
MYSQL_WRAP *sql;

static RES_ROWS_ITER *setup_sql_read(const char *query, ...) {
    va_list args;
    va_start(args, query);
    return sql_read(sql, query, args);
}

static void del_key(const char *key) {
    void *reply = redisCommand(redis_wrap_get_context(redis_wrap), "DEL %s", key);
    freeReplyObject(reply);
}

static int32_t setup_redis_write(const char *key) {
    if(key)
        del_key(key);

    char *query = "select first_name, last_name from employees limit 2";
    RES_ROWS_ITER *iter = setup_sql_read(query);
    return redis_write(redis_wrap, key, iter);
    res_row_iter_free(iter);
}

void before_all() {
    MYSQL *mysql = mysql_init(NULL);
    char *MYSQL_HOST = getenv("MYSQL_HOST");
    char *MYSQL_USER = getenv("MYSQL_USER");
    char *MYSQL_PASS = getenv("MYSQL_PASS");
    char *REDIS_HOST = getenv("REDIS_HOST");
    char *REDIS_PORT = getenv("REDIS_PORT");

    if(mysql_real_connect(mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, "employees", 0, NULL, 0) == NULL) {
        fprintf(stderr, "MYSQL Connection failed, make sure to run setup_db.sh in scripts folder\n"); 
        exit(1);
    }

    sql = mysql_wrap_init(mysql);

    redisContext *context = redisConnect(REDIS_HOST, atoi(REDIS_PORT));
    redis_wrap = redis_wrap_init(context);
}

void simple_redis_wrap_read(CuTest *tc) {

    setup_redis_write("key");

    RES_ROWS_ITER *iter = redis_read(redis_wrap, "key");

    CuAssertIntEquals(tc, 2, res_row_iter_rows(iter));
    CuAssertIntEquals(tc, 2, res_row_iter_cols(iter));

    CuAssertTrue(tc, res_row_iter_has_next(iter));
    char **next = res_row_iter_next(iter);
    CuAssertStrEquals(tc, "Georgi", next[0]);
    CuAssertStrEquals(tc, "Facello", next[1]);

    CuAssertTrue(tc, res_row_iter_has_next(iter));

    next = res_row_iter_next(iter);

    CuAssertStrEquals(tc, "Bezalel", next[0]);
    CuAssertStrEquals(tc, "Simmel", next[1]);

    CuAssertTrue(tc, !res_row_iter_has_next(iter));

    res_row_iter_free(iter);
}

void simple_redis_wrap_write(CuTest *tc) {
    int32_t res = setup_redis_write("key");
    //2 rows hould have been inserted
    CuAssertIntEquals(tc, 2, res);
}

//redis_write edge cases
void redis_wrap_write_null_key(CuTest *tc) {
    setup_redis_write(NULL);
    CuAssertTrue(tc, redis_wrap_get_err(redis_wrap) != NULL);
}

void redis_wrap_write_null_iter(CuTest *tc) {
    redis_write(redis_wrap, "somekey", NULL);
    CuAssertTrue(tc, redis_wrap_get_err(redis_wrap) != NULL);
}

void redis_wrap_write_null_wrap(CuTest *tc) {
    redis_write(NULL, "somekey", NULL);
    CuAssertTrue(tc, redis_wrap_get_err(redis_wrap) != NULL);
}


//redis_read edge cases

void redis_wrap_read_null_key(CuTest *tc) {
    RES_ROWS_ITER *iter = redis_read(redis_wrap, NULL);
    CuAssertTrue(tc, iter == NULL);
    CuAssertTrue(tc, redis_wrap_get_err(redis_wrap) != NULL);

}

void redis_wrap_read_null_wrap(CuTest *tc) {
    RES_ROWS_ITER *iter = redis_read(NULL, "somekey");
    CuAssertTrue(tc, iter == NULL);
    char *res = redis_wrap_get_err(NULL);
    CuAssertStrEquals(tc, ERR_NULL_WRAP_STR, res);
}

//other edge cases
void redis_wrap_null_get_err(CuTest *tc) {
    char *res = redis_wrap_get_err(NULL);
    CuAssertTrue(tc, res != NULL);
}

//just to see that it doesnt crash
void test_redis_wrap_free(CuTest *tc) {
    redis_wrap_free(redis_wrap);
}

extern CuSuite *redis_api_suite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, simple_redis_wrap_read);
    SUITE_ADD_TEST(suite, simple_redis_wrap_write);
    SUITE_ADD_TEST(suite, redis_wrap_write_null_key);
    SUITE_ADD_TEST(suite, test_redis_wrap_free);
    SUITE_ADD_TEST(suite, redis_wrap_write_null_iter);
    SUITE_ADD_TEST(suite, redis_wrap_write_null_wrap);
    SUITE_ADD_TEST(suite, redis_wrap_null_get_err);

    SUITE_ADD_TEST(suite, redis_wrap_read_null_key);
    SUITE_ADD_TEST(suite, redis_wrap_read_null_wrap);

    
    return suite;
}
