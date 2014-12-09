#include "../include/zpipe.h"
#include "../include/ztest.h"
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
    EQUAL(u, (vector<int> {4, 8}));
    EQUAL(ifrom(u) | isum(), 12);

    const char* s = "ABC";
    string t = ifrom(s) | iconcat(", ");
    EQUAL(t, "A, B, C")

    vector<string> vs {"1", "2", "3"};
    string vt = ifrom(vs) | iconcat('|');
    EQUAL(vt, "1|2|3")

    string vr = ifrom(vs) | iconcat('|', 2);
    EQUAL(vr, "1||2||3")

    int A[] {1, 2, 3, 4, 5};
    int m = ifrom(A + 2, A + 3) | isum();
    EQUAL(m, 3)

    EQUAL(ifrom({1, 2, 2, 4}) | icount(2), 2)

END
