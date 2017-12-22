#include "../../CuTest.h"
#include "../redsql.h"


redsql_conn *r_conn;
redisContext *context;
#define QUERY "select emp_no, first_name, last_name, gender, salary from employees natural join dept_emp natural join salaries where gender = '%s' and salary > %d limit 3"
#define KEY "redsql_key"

void before_all() {
    char *MYSQL_HOST = getenv("MYSQL_HOST");
    char *MYSQL_USER = getenv("MYSQL_USER");
    char *MYSQL_PASS = getenv("MYSQL_PASS");
    char *REDIS_HOST = getenv("REDIS_HOST");
    int REDIS_PORT = atoi(getenv("REDIS_PORT"));

    r_conn = establish_conn(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, 
            "employees", REDIS_HOST, REDIS_PORT);
    context = redisConnect(REDIS_HOST, REDIS_PORT);
}

//READ/WRITE TEST
void redsql_test_simple_read(CuTest *tc) {
    RES_ROWS_ITER *iter = redsql_read(r_conn, KEY, QUERY, true, "M", 50000);
    void *res = redisCommand(context, "FLUSHALL");
    freeReplyObject(res);

    CuAssertTrue(tc, iter != NULL);
    CuAssertIntEquals(tc, 3, res_row_iter_rows(iter));
    res_row_iter_free(iter);
}

void redsql_test_simple_write(CuTest *tc) {

}
void redsql_test_read(CuTest *tc) {
    //check that key is not in cache
    bool in_cache = redsql_in_cache(r_conn, KEY);
    CuAssertTrue(tc, !in_cache);

    RES_ROWS_ITER *iter = redsql_read(r_conn, KEY, QUERY, true, "M", 50000);
    //check that key is in cache
    in_cache = redsql_in_cache(r_conn, KEY);

    //evict key from cache
    int num = redsql_evict(r_conn, KEY);
    char **row = res_row_iter_next(iter);

    //put all assertions at the the end to maintain constant state of application
    CuAssertTrue(tc, num == 1);
    CuAssertTrue(tc, in_cache);
    CuAssertIntEquals(tc, 3, res_row_iter_rows(iter));
    CuAssertStrEquals(tc, "Shir", row[1]);
    CuAssertStrEquals(tc, "McClurg", row[2]);
    CuAssertStrEquals(tc, "56473", row[4]);
}


//EVICTION TESTS
void redsql_test_eviction(CuTest *tc) {
    redsql_read(r_conn, KEY, QUERY, true, "M", 50000);
    int num = redsql_evict(r_conn, KEY);
    CuAssertIntEquals(tc, 1, num);
}

void redsql_test_no_eviction(CuTest *tc) {
    int num = redsql_evict(r_conn, "random_key");
    CuAssertIntEquals(tc, 0, num);
}

//IN_CACHE TESTS
void redsql_test_in_cache(CuTest *tc) {

    redsql_read(r_conn, "user", QUERY, true, "M", 50000);
    bool in_cache = redsql_in_cache(r_conn, "user");
    redsql_evict(r_conn, "user");

    CuAssertTrue(tc, in_cache);
    CuAssertTrue(tc, !redsql_in_cache(r_conn, "user"));
}

void redsql_test_not_in_cache(CuTest *tc) {
    bool in_cache = redsql_in_cache(r_conn, KEY);
    CuAssertTrue(tc, !in_cache);
}

//FREE TEST
void redsql_test_free(CuTest *tc) {
    free_redsql_conn(r_conn);
}


extern CuSuite *redsql_suite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, redsql_test_simple_read);
    SUITE_ADD_TEST(suite, redsql_test_simple_write);

    SUITE_ADD_TEST(suite, redsql_test_eviction);
    SUITE_ADD_TEST(suite, redsql_test_no_eviction);

    SUITE_ADD_TEST(suite, redsql_test_in_cache);
    SUITE_ADD_TEST(suite, redsql_test_not_in_cache);

    SUITE_ADD_TEST(suite, redsql_test_read);
    SUITE_ADD_TEST(suite, redsql_test_free);

    return suite;
}
