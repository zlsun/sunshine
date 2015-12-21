#include "../include/zinit.h"
#include "../include/ztest.h"

int a;
int b;

INIT_BEGIN {
    a = 1;
    b = a + 1;
} INIT_END

TEST(init)
{
    EQUAL(a, 1);
    EQUAL(b, 2);
}

TEST_MAIN
