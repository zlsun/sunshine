
#include "zmacro.h"
#include "ztest.h"

TEST(CONCATENATE)
{
#define a b
#define M 2
    int CONCATENATE(a, 2) = 1;
    int CONCATENATE_IMPL(a, M) = 2;
    EQUAL(CONCATENATE(a, 2), b2)
    EQUAL(CONCATENATE_IMPL(a, M), aM)
#undef a
#undef M
}

TEST(ARGS_SIZE)
{
    EQUAL(ARGS_SIZE(), 0)
    EQUAL(ARGS_SIZE(1), 1)
    EQUAL(ARGS_SIZE(1, 2), 2)
    EQUAL(ARGS_SIZE(1, 2, 3), 3)
    EQUAL(ARGS_SIZE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 10)

    EQUAL(ARGS_SIZE(a), 1)
}

TEST(ARGS_SIZE_V2)
{
    EQUAL(ARGS_SIZE_V2(), 0)
    EQUAL(ARGS_SIZE_V2(1), 1)
    EQUAL(ARGS_SIZE_V2(1, 2), 2)
    EQUAL(ARGS_SIZE_V2(1, 2, 3), 3)
    EQUAL(ARGS_SIZE_V2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 10)

    // EQUAL(ARGS_SIZE_V2(a), 1)
}

TEST_MAIN
