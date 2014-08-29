#include <sstream>
#include "../ztest.h"
#include "../zlog.h"
using namespace std;


test_begin(zlog_test);
    ostringstream ss;
    string s;

    #define check(valid)       \
        s = ss.str();          \
        test_equal(s, valid);  \
        s.clear();             \
        ss.str(s);

    zlogo(ss) 1, 2, 3, 4;
    check("1 2 3 4\n");

    zlogo(ss) vector<int> {1, 2, 3};
    check("[1, 2, 3]\n");

    zlogf("%d %-10d %% %d", ss) 1, 44;
    check("1 44         %% %d\n");

test_end();

test_main();
