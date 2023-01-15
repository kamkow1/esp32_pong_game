#include <stdio.h>
#include <stddef.h>

#include "lib.h"

#define TESTS_COUNT 1

int main() 
{
    TestResult tests[TESTS_COUNT];
    run_all_tests(tests);

    printf("TEST RESULTS:\n");
    for (size_t i = 0; i < TESTS_COUNT; i++) {
        TestResult tr = tests[i];
        char *msg = tr.return_code == 0 ? "PASSED" : "FAILED";
        printf("TEST RESULT [id: %d][name: %s][rc: %d]: %s\n", i, tr.test_name, tr.return_code, msg);
    }

    return 0;
}
