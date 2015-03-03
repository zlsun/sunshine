#include "../include/zpipe.h"
#include "../include/ztest.h"
#include "../include/zlog.h"
using namespace std;
using namespace zl;

TEST(zpipe)
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
END

TEST(zpipe_aggregate)
    int A[] {1, 2, 3, 4, 5};
    EQUAL(ifrom(A) | imax, 5)
    EQUAL(ifrom(A) | imin, 1)
    EQUAL(ifrom(A + 2, A + 3) | isum(), 3)
    EQUAL(ifrom({1, 2, 2, 4}) | icount(2), 2)
END

TEST(zpipe_concat)
    const char* s = "ABC";
    EQUAL(ifrom(s) | iconcat(", "), "A, B, C")
    vector<string> vs {"1", "2", "3"};
    EQUAL(ifrom(vs) | iconcat('|'), "1|2|3")
    EQUAL(ifrom(vs) | iconcat('|', 2), "1||2||3")
END

TEST(zpipe_range)
    EQUAL(irange(10) | isum(), 45)
    EQUAL(irange(10) | isum(2), 47)
    EQUAL(irange(2, 10) | isum(), 44)
    EQUAL(irange(1, 10, 2) | isum(), 25)
    EQUAL(irange(10, 1, -2) | isum(), 30)
    EQUAL(irange('a', 'c' + 1) | iconcat('|'), "a|b|c")
END

TEST(zpipe_print)
    ostringstream ss;
    string s;

    #define check(right) \
        s = ss.str();    \
        EQUAL(s, right)  \
        s.clear();       \
        ss.str(s);

    zlogo(ss) ifrom({1, 2, 3});
    check("[1, 2, 3]\n");

    zlogo(ss) ifrom({1, 2, 3}) | iselect([](int i) {
        return i + 1;
    });
    check("[2, 3, 4]\n");

    zlogo(ss) irange(3);
    check("[0, 1, 2]\n");

    zlogo(ss) irepeat(1, 3);
    check("[1, 1, 1]\n");
END

TEST(zpipe_algorithm)
    EQUAL(ifrom({3, 2, 4, 1}) | ireverse | to_vector, (vector<int> {1, 4, 2, 3}))
    EQUAL(ifrom({3, 2, 4, 1}) | isort | to_vector, (vector<int> {1, 2, 3, 4}))
END
