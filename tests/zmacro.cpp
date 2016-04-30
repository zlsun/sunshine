
#include "zmacro.h"
#include "ztest.h"

#define a b
#define M 2

TEST(CONCATENATE)
{
    int CONCATENATE(a, 2) = 1;
    int CONCATENATE_IMPL(a, M) = 2;
    EQUAL(CONCATENATE(a, 2), b2)
    EQUAL(CONCATENATE_IMPL(a, M), aM)
}

TEST(ARGS_SIZE)
{
    EQUAL(ARGS_SIZE(), 0)
    EQUAL(ARGS_SIZE(1), 1)
    EQUAL(ARGS_SIZE(1, 2), 2)
    EQUAL(ARGS_SIZE(1, 2, 3), 3)
    EQUAL(ARGS_SIZE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 10)
}

TEST_MAIN
