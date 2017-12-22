#include "../../CuTest.h"
#include "../redsql.h"


redsql_conn *r_conn;

void before_all() {
    char *MYSQL_HOST = getenv("MYSQL_HOST");
    char *MYSQL_USER = getenv("MYSQL_USER");
    char *MYSQL_PASS = getenv("MYSQL_PASS");
    char *REDIS_HOST = getenv("REDIS_HOST");
    char *REDIS_PORT = getenv("REDIS_PORT");

    r_conn = establish_conn(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, 
            "employees", REDIS_HOST, atoi(REDIS_PORT));
}

//READ/WRITE TEST
void redsql_simple_read(CuTest *tc) {

}

void redsql_simple_write(CuTest *tc) {

}

//EVICTION TESTS
void redsql_test_eviction(CuTest *tc) {

}

void redsql_test_no_eviction(CuTest *tc) {

}

//IN_CACHE TESTS
void redsql_test_in_cache(CuTest *tc) {

}

void redsql_test_not_in_cache(CuTest *tc) {

}

//FREE TEST
void redsql_test_free(CuTest *tc) {

}


extern CuSuite *redsql_suite() {
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, redsql_simple_read);
    SUITE_ADD_TEST(suite, redsql_simple_write);
    SUITE_ADD_TEST(suite, redsql_test_eviction);
    SUITE_ADD_TEST(suite, redsql_test_no_eviction);
    SUITE_ADD_TEST(suite, redsql_test_in_cache);
    SUITE_ADD_TEST(suite, redsql_test_not_in_cache);
    SUITE_ADD_TEST(suite, redsql_test_free);

    return suite;
}
