#include <stdio.h>
#include "../../CuTest.h"

extern CuSuite *redis_api_suite();
extern void before_all();

static void get_res(CuSuite *suite) {

    CuString *res = CuStringNew();
    CuSuiteSummary(suite, res);
    CuSuiteDetails(suite, res);

    printf("%s\n", res->buffer);
}

int main(void) {
    before_all();
    CuSuite *suite = CuSuiteNew();

    CuSuiteAddSuite(suite, redis_api_suite());
    CuSuiteRun(suite);
    
    get_res(suite);

    return 0;
}
