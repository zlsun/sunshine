#include "zinit.h"
#include "ztest.h"

int a;
int b;

INIT
{
    a = 1;
    b = a + 1;
}

TEST(init)
{
    EQUAL(a, 1)
    EQUAL(b, 2)
}

TEST_MAIN
