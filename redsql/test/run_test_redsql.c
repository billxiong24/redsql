#include <stdio.h>
#include "../../test_functions/test_func.h"

extern CuSuite *redsql_suite();

int main(void) {
    test_run(redsql_suite);
    return 0;
}
