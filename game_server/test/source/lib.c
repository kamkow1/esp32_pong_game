#include <stdlib.h>

#include "lib.h"
#include "client_send.h"

static int TEST_COUNTER = 0;

#define APPEND_TEST(ts, t)  \
{                           \
    ts[TEST_COUNTER++] = t; \
}                           \

void run_all_tests(TestResult *test_results)
{
    TestResult cst_tr1;
    client_send_test(&cst_tr1);
    APPEND_TEST(test_results, cst_tr1);
}
