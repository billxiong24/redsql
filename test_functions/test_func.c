#include "test_func.h"
#include <stdio.h>

//expect file including this to supply before_all function
extern void before_all();

static void get_res(CuSuite *suite) {

    CuString *res = CuStringNew();
    CuSuiteSummary(suite, res);
    CuSuiteDetails(suite, res);

    printf("%s\n", res->buffer);
}

void test_run(New_suite func) {
    before_all();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, func());
    CuSuiteRun(suite);
    
    get_res(suite);
}
