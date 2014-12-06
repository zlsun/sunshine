#include <sstream>
#include "../include/ztest.h"
#include "../include/zlog.h"
using namespace std;

test_begin(zlog_test);
    ostringstream ss;
    string s;

    #define check(right)       \
        s = ss.str();          \
        test_equal(s, right);  \
        s.clear();             \
        ss.str(s);

    zlogo(ss) 1, 2, 3, 4;
    check("1 2 3 4\n");

    zlogo(ss) vector<int> {1, 2, 3};
    check("[1, 2, 3]\n");

    zlogf("%1% %2% %3% %%", ss) 1, 44, 1.0;
    check("1 44 1 %\n");

    zlogf("%d %-4d %.3f %%", ss) 1, 44, 1.0;
    check("1 44   1.000 %\n");

    zlogf("%1$d %2$-4d %3$.3f %%", ss) 1, 44, 1.0;
    check("1 44   1.000 %\n");

test_end();

test_main();
