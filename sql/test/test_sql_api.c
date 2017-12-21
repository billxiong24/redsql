#include "../../CuTest.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include "../sql_api.h"
#include <stdarg.h>


MYSQL_WRAP *sql;
void before_all() {
    MYSQL *mysql = mysql_init(NULL);
    char *MYSQL_HOST = getenv("MYSQL_HOST");
    char *MYSQL_USER = getenv("MYSQL_USER");
    char *MYSQL_PASS = getenv("MYSQL_PASS");

    if(mysql_real_connect(mysql, MYSQL_HOST, MYSQL_USER, MYSQL_PASS, "employees", 0, NULL, 0) == NULL) {
        fprintf(stderr, "MYSQL Connection failed, make sure to run setup_db.sh in scripts folder\n"); 
        exit(1);
    }
    sql = mysql_wrap_init(mysql);
}

static RES_ROWS_ITER *setup_sql_read(const char *query, ...) {
    va_list args;
    va_start(args, query);
    return sql_read(sql, query, args);
}

static int32_t setup_sql_write(const char *query, ...) {
    va_list args;
    va_start(args, query);

    return sql_write(sql, query, args);
}

static char *test_gen_query(const char *query, ...) {
    va_list args;
    va_start(args, query);

    char *buf = gen_query(query, args);
    return buf;
}

void simple_query_gen(CuTest *tc) {
    char *query = test_gen_query("select emp_no, last_name, hire_date, dept_no from employees natural join dept_manager where gender='%s' and last_name='%s'", "M", "Test");

    char *res = "select emp_no, last_name, hire_date, dept_no from employees natural join dept_manager where gender='M' and last_name='Test'";

    CuAssertStrEquals(tc, res, query);
}

void simple_sql_read(CuTest *tc) {
    RES_ROWS_ITER *iter = setup_sql_read("select first_name, last_name from employees limit %d", 2);
    char **next = res_row_iter_next(iter);
    CuAssertStrEquals(tc, next[0], "Georgi");
    CuAssertStrEquals(tc, next[1], "Facello");

    next = res_row_iter_next(iter);
    CuAssertStrEquals(tc, next[0], "Bezalel");
    CuAssertStrEquals(tc, next[1], "Simmel");
    
    CuAssertTrue(tc, !res_row_iter_has_next(iter));
}

void complex_sql_read(CuTest *tc) {
    char *query = "select emp_no, last_name, hire_date, dept_no from employees natural join dept_manager where gender='%s' and last_name='%s'";
    
    RES_ROWS_ITER *iter = setup_sql_read(query, "F", "Sigstam");
    CuAssertIntEquals(tc, res_row_iter_rows(iter), 1);
    CuAssertIntEquals(tc, res_row_iter_cols(iter), 4);

    char **next = res_row_iter_next(iter);

    CuAssertStrEquals(tc, "110228", next[0]);
    CuAssertStrEquals(tc, "Sigstam", next[1]);
    CuAssertStrEquals(tc, "1985-08-04", next[2]);
    CuAssertStrEquals(tc, "d003", next[3]);
    
    CuAssertTrue(tc, !res_row_iter_has_next(iter));
}


void sql_read_metadata(CuTest *tc) {
    RES_ROWS_ITER *iter = setup_sql_read("select * from employees limit %d", 5);

    CuAssertIntEquals(tc, 5, res_row_iter_rows(iter));
    CuAssertIntEquals(tc, 6, res_row_iter_cols(iter));
}

void edge_sql_read(CuTest *tc) {
    RES_ROWS_ITER *iter = setup_sql_read(NULL, 4, 5, 5, 5, 5);
    CuAssertPtrEquals(tc, NULL, iter);
}

void simple_sql_update(CuTest *tc) {
    char *query = "update employees set last_name='%s' where emp_no=110228";

    int32_t res = setup_sql_write(query, "Smith");
    int32_t res2 = setup_sql_write(query, "Sigstam");
    CuAssertIntEquals(tc, 1, res);
    CuAssertIntEquals(tc, 1, res2);
}

void simple_sql_insert_delete(CuTest *tc) {
    char *query = "insert into employees values (%d, '%s', '%s', '%s', '%s', '%s')";
    
    int32_t res = setup_sql_write(query, 123,  "1996-12-12", "adf", "adsf", "F", "2010-01-01");

    //reset to original state
    int32_t res2 = setup_sql_write("delete from employees where emp_no=%d", 123);
    CuAssertIntEquals(tc, 1, res);
    CuAssertIntEquals(tc, 1, res2);
}

void edge_sql_write(CuTest *tc) {
    int32_t res = setup_sql_write(NULL, "a");
    CuAssertTrue(tc, res < 0);
    CuAssertTrue(tc, mysql_wrap_get_err(sql) != NULL);
}

void null_args_write(CuTest *tc) {

    va_list args;
    int32_t res = sql_write(NULL, "wtest", args);
    CuAssertTrue(tc, res < 0);

    res = sql_write(sql, "some query", NULL);
    CuAssertTrue(tc, res < 0);
    //the error was placed into the MYSQL_WRAP *struct
    CuAssertTrue(tc, mysql_wrap_get_err(sql) != NULL);
}

void free_wrap_test(CuTest *tc) {
    //just to see that it doesnt crash
    mysql_wrap_free(sql);
}

extern CuSuite *sql_api_suite() {
    CuSuite *suite = CuSuiteNew();
    
    //gen_query tests
    SUITE_ADD_TEST(suite, simple_query_gen);

    //sql_read tests
    SUITE_ADD_TEST(suite, simple_sql_read);
    SUITE_ADD_TEST(suite, complex_sql_read);
    SUITE_ADD_TEST(suite, sql_read_metadata);
    SUITE_ADD_TEST(suite, edge_sql_read);

    //sql_write tests
    SUITE_ADD_TEST(suite, simple_sql_update);
    SUITE_ADD_TEST(suite, simple_sql_insert_delete);
    SUITE_ADD_TEST(suite, edge_sql_write);
    SUITE_ADD_TEST(suite, null_args_write);

    //free
    SUITE_ADD_TEST(suite, free_wrap_test);

    return suite;
}
