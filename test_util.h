#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdio.h>

static int g_test_fail_count = 0;

#define UT_EQ_INT(expected, actual) do { \
    int _e = (expected), _a = (actual); \
    if (_e != _a) { \
        printf("FAIL %s() %s:%d: expected %d but got %d\n", __func__, __FILE__, __LINE__, _e, _a); \
        g_test_fail_count++; \
    } \
} while (0)

#define UT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("FAIL %s() %s:%d: expected true\n", __func__, __FILE__, __LINE__); \
        g_test_fail_count++; \
    } \
} while (0)

#endif
