#include <sstream>
#include "../include/ztest.h"
#include "../include/zlog.h"
using namespace std;

TEST(zlog)
{
    ostringstream ss;
    string s;

#define check(right) \
        s = ss.str();    \
        EQUAL(s, right)  \
        s.clear();       \
        ss.str(s);

    zlogo(ss) 1, 2, 3, 4;
    check("1 2 3 4\n")

    zlogo(ss, false) 1, 2, 3, 4;
    check("1 2 3 4")

    zlogo(ss, false, false) 1, 2, 3, 4;
    check("1234")

    zlogo(ss) vector<int> {1, 2, 3};
    check("[1, 2, 3]\n")

    zlogfo("%1% %2% %3% %%", ss) 1, 44, 1.0;
    check("1 44 1 %\n")

    zlogfo("%d %-4d %.3f %%", ss) 1, 44, 1.0;
    check("1 44   1.000 %\n")

    zlogfo("%1$d %2$-4d %3$.3f %%", ss) 1, 44, 1.0;
    check("1 44   1.000 %\n")

    zlogfo("%1$d %2$-4d %3$.3f %%", ss, false) 1, 44, 1.0;
    check("1 44   1.000 %")

}

TEST_MAIN
