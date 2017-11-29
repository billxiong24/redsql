#include <stdio.h>
#include "../../CuTest.h"

extern CuSuite *sql_api_suite();
extern void simple_sql_read(CuTest *tc);
extern void before_all();
extern void after_all();

static void get_res(CuSuite *suite) {

    CuString *res = CuStringNew();
    CuSuiteSummary(suite, res);
    CuSuiteDetails(suite, res);

    printf("%s\n", res->buffer);
}

int main(void) {
    before_all();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, sql_api_suite());
    CuSuiteRun(suite);
    
    get_res(suite);

    return 0;
}
