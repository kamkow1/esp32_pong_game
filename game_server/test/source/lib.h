#ifndef LIB_H
#define LIB_H

#include <stddef.h>

#define TEST_FAILED                 \
{                                   \
    test_result->return_code = 1;    \
}                                   \

#define TEST_PASSED                 \
{                                   \
    return;                         \
}                                   \

typedef struct {
    int  return_code;
    char *test_name;
} TestResult;

void run_all_tests(TestResult *test_results);

#endif
