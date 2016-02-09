#include <sstream>

#include "../include/zlog.h"
#include "../include/ztest.h"

using namespace std;

#define check(right)         \
    {                        \
        string s = ss.str(); \
        EQUAL(s, right)      \
        s.clear();           \
        ss.str(s);           \
    }

TEST(zlog)
{
    ostringstream ss;
    zl::Logger::to(ss);

    zlog 1, 2, 3, 4;
    check("1 2 3 4\n")

    zloga(false) 1, 2, 3, 4;
    check("1 2 3 4")

    zloga(false, false) 1, 2, 3, 4;
    check("1234")

    zlog vector<int> {1, 2, 3};
    check("[1, 2, 3]\n")

}

TEST_MAIN
