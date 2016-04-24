#include "zproperty.h"
#include "ztest.h"

using namespace zl;

auto getter = [](auto& prop) -> auto& {
    ++prop.getAdd().gc;
    return prop.getObj();
};

auto setter = [](auto& prop, const int& ni) {
    ++prop.getAdd().sc;
    prop.getObj() = ni;
};

class A
{
public:
    Property<int> i = 0;
    struct Counter { int gc = 0, sc = 0; };
    Property<int, Counter> j {0, getter, setter};

    // Test constuctor
    Property<int, Counter> k1 {};
    Property<int, Counter> k2 {0};
    Property<int, Counter> k3 {getter};
    Property<int, Counter> k4 {0, getter};
    Property<int, Counter> k5 {setter};
    Property<int, Counter> k6 {0, setter};
    Property<int, Counter> k7 {getter, setter};
    Property<int, Counter> k8 {0, getter, setter};
};

TEST(property_get)
{
    A a;
    EQUAL(a.i.get(), 0)
    EQUAL(a.i, 0)
    a.j.get();
    EQUAL(a.j.getAdd().gc, 1)
    a.j.get();
    EQUAL(a.j.getAdd().gc, 2)
}

TEST(property_set)
{
    A a;
    a.i.set(1);
    EQUAL(a.i, 1)
    a.i = 2;
    EQUAL(a.i, 2)
    a.j = 1;
    EQUAL(a.j.getAdd().sc, 1)
    a.j = 2;
    EQUAL(a.j.getAdd().sc, 2)
}

TEST_MAIN
