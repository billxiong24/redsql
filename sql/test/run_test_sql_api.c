#include "../../test_functions/test_func.h"

extern CuSuite *sql_api_suite();

int main(void) {
    test_run(sql_api_suite);
    return 0;
}
