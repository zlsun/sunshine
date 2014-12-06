#include "../include/zpipe.h"
#include "../include/ztest.h"
using namespace std;
using namespace zl;

test_begin(zpipe);
    const vector<int> v {1, 2, 3};
    auto u = ifrom(v)
           | iwhere([](int i) {
               return i % 2 == 1;
           })
           | iselect([](int i) {
               return (i + 1) * 2;
           })
           | to_vector;
    test_equal(u, (vector<int> {4, 8}));
    test_equal(ifrom(u) | isum(), 12);

    const char* s = "ABC";
    string t = ifrom(s) | iconcat(", ");
    test_equal(t, "A, B, C")

    vector<string> vs {"1", "2", "3"};
    string vt = ifrom(vs) | iconcat('|');
    test_equal(vt, "1|2|3");

    int A[] {1, 2, 3, 4, 5};
    int m = ifrom(A + 2, A + 3) | isum();
    test_equal(m, 3);

    test_equal(ifrom({1, 2, 2, 4}) | icount(2), 2)

test_end();

test_main();