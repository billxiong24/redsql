#include "../../CuTest.h"
#include <mysql/mysql.h>
#include <stdio.h>
#include "../sql_api.h"
#include <stdarg.h>


MYSQL *sql;
void before_all() {
    sql = mysql_init(NULL);

    if(mysql_real_connect(sql, "localhost", "root", "Chem1313#", "employees", 0, NULL, 0) == NULL) {
        /**
         * Something has gone wrong, this probably means table was not created.
         * Otherwise, double check credentials
         */
        fprintf(stderr, "MYSQL Connection failed, make sure to run setup_db.sh in scripts folder\n"); 
        exit(1);
    }
}

static RES_ROWS_ITER *setup_sql_read(const char *query, ...) {
    va_list args;
    va_start(args, query);
    return sql_read(sql, query, args);
}

static uint32_t setup_sql_write(const char *query, ...) {
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
    uint32_t res = setup_sql_write(query, "Smith");

    CuAssertIntEquals(tc, 1, res);

    //reset to original name
    res = setup_sql_write(query, "Sigstam");
    CuAssertIntEquals(tc, 1, res);
}

void simple_sql_insert_delete(CuTest *tc) {
    char *query = "insert into employees values (%d, '%s', '%s', '%s', '%s', '%s')";
    
    uint32_t res = setup_sql_write(query, 123,  "1996-12-12", "adf", "adsf", "F", "2010-01-01");
    CuAssertIntEquals(tc, 1, res);

    //reset to original state
    res = setup_sql_write("delete from employees where emp_no=%d", 123);
    CuAssertIntEquals(tc, 1, res);
}

void edge_sql_write(CuTest *tc) {
    uint32_t res = setup_sql_write(NULL, "a");
    CuAssertIntEquals(tc, 0, res);
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

    return suite;
}
