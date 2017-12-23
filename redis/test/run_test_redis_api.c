#include <stdio.h>
#include "../../test_functions/test_func.h"

extern CuSuite *redis_api_suite();

int main(void) {
    test_run(redis_api_suite);
    
    return 0;
}
