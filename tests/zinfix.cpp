#include "../include/zinfix.h"
#include "../include/ztest.h"

INFIX(add, int(int, int))
{
    return lhs + rhs;
}

INFIX(mul, int(int, int))
{
    return lhs * rhs;
}

TEST(add)
{
    EQUAL(1 |add| 2, 3)
}

TEST(mul)
{
    EQUAL(1 *mul* 2, 2)
}

TEST(mix)
{
    EQUAL(1 +add+ 2 *mul* 3, 7)
}

TEST_MAIN
