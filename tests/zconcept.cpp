#include "ztest.h"
#include "zconcept.h"
using namespace zl;

struct A {};

TEST(CanCout)
{
    EQUAL(CanCout<int>, true)
    EQUAL(CanCout<A>, false)
}

TEST_MAIN
