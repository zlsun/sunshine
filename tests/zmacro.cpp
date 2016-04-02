
#include "zmacro.h"
#include "ztest.h"

TEST(ARGS_SIZE)
{
    EQUAL(ARGS_SIZE(), 0)
    EQUAL(ARGS_SIZE(1), 1)
    EQUAL(ARGS_SIZE(1, 2), 2)
    EQUAL(ARGS_SIZE(1, 2, 3), 3)
    EQUAL(ARGS_SIZE(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), 10)
}

TEST_MAIN
