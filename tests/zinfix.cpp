#include "../include/zinfix.h"
#include "../include/ztest.h"

INFIX(add, int(int, int)) {
    return lhs + rhs;
}

TEST(infix_add) {
    EQUAL(1 |add| 2, 3)
}

TEST_MAIN
