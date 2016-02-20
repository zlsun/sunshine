#include "../include/zpipe.h"
#include "../include/ztest.h"
#include "../include/zlog.h"

using namespace std;
using namespace zl;

TEST(basic)
{
    const vector<int> v {1, 2, 3};
    auto u = ifrom(v)
           | iwhere([](int i) {
               return i % 2 == 1;
           })
           | iselect([](int i) {
               return (i + 1) * 2;
           })
           | to_vector;
    EQUAL(u, (vector<int> {4, 8}))

    EQUAL(ifrom({1, 2, 3, 0}) | iall([](int i) {
        return i > 0;
    }), false)

    EQUAL(ifrom({1, 2, 3, 0}) | iany([](int i) {
        return i == 0;
    }), true)

    EQUAL(irepeat(1, 10) | isum(), 10)
}

TEST(aggregate)
{
    int A[] {1, 2, 3, 4, 5};
    EQUAL(ifrom(A) | imax, 5)
    EQUAL(ifrom(A) | imin, 1)
    EQUAL(ifrom(A + 2, A + 3) | isum(), 3)
    EQUAL(ifrom({1, 2, 2, 4}) | icount(2), 2)
}

TEST(concat)
{
    const char* s = "ABC";
    EQUAL(ifrom(s) | iconcat(", "), "A, B, C")
    vector<string> vs {"1", "2", "3"};
    EQUAL(ifrom(vs) | iconcat('|'), "1|2|3")
    EQUAL(ifrom(vs) | iconcat('|', 2), "1||2||3")
}

TEST(range)
{
    EQUAL(irange(10) | isum(), 45)
    EQUAL(irange(10) | isum(2), 47)
    EQUAL(irange(2, 10) | isum(), 44)
    EQUAL(irange(1, 10, 2) | isum(), 25)
    EQUAL(irange(10, 1, -2) | isum(), 30)
    EQUAL(irange('a', 'c' + 1) | iconcat('|'), "a|b|c")
}

#define check(right)         \
    {                        \
        string s = ss.str(); \
        EQUAL(s, right)      \
        s.clear();           \
        ss.str(s);           \
    }

TEST(print)
{
    ostringstream ss;
    zl::Logger::to(ss);

    zlog ifrom({1, 2, 3});
    check("[1, 2, 3]\n");

    zlog ifrom({1, 2, 3}) | iselect([](int i) {
        return i + 1;
    });
    check("[2, 3, 4]\n");

    zlog irange(3);
    check("[0, 1, 2]\n");

    zlog irepeat(1, 3);
    check("[1, 1, 1]\n");
}

TEST(algorithm)
{
    EQUAL(ifrom({3, 2, 4, 1}) | ireverse | to_vector, (vector<int> {1, 4, 2, 3}))
    EQUAL(ifrom({3, 2, 4, 1}) | isort | to_vector, (vector<int> {1, 2, 3, 4}))
}

TEST_MAIN
