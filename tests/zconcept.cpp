#include "ztest.h"
#include "zconcept.h"
using namespace zl;

struct A
{
};
struct B
{
    friend std::ostream& operator << (std::ostream& os, const B&)
    {
        return os;
    }
};

TEST(CanCout)
{
    EQUAL(CanCout<int>, true)
    EQUAL(CanCout<A>, false)
    EQUAL(CanCout<B>, true)
}

TEST_MAIN
