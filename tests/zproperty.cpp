#include "zproperty.h"
#include "ztest.h"

using namespace zl;

class A
{
public:
    Property<int> i = 0;
};

class B
{
public:
    struct Counter { int gc = 0, sc = 0; };
    Property<int, Counter> i {
        0,
        [](auto& prop) -> int& {
            ++prop.getAdd().gc;
            return prop.getObj();
        },
        [](auto& prop, const int& ni) {
            ++prop.getAdd().sc;
            prop.getObj() = ni;
        }
    };
};

TEST(property_get)
{
    A a;
    EQUAL(a.i.get(), 0)
    EQUAL(a.i, 0)
    B b;
    b.i.get();
    EQUAL(b.i.getAdd().gc, 1)
    b.i.get();
    EQUAL(b.i.getAdd().gc, 2)
}

TEST(property_set)
{
    A a;
    a.i.set(1);
    EQUAL(a.i, 1)
    a.i = 2;
    EQUAL(a.i, 2)
    B b;
    b.i = 1;
    EQUAL(b.i.getAdd().sc, 1)
    b.i = 2;
    EQUAL(b.i.getAdd().sc, 2)
}

TEST_MAIN
