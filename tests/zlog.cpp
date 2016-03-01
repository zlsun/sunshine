#include <sstream>

#include "zlog.h"
#include "zinit.h"
#include "ztest.h"

using namespace std;

#define check(right)         \
    {                        \
        string s = ss.str(); \
        EQUAL(s, right)      \
        s.clear();           \
        ss.str(s);           \
    }

struct A {};

ostringstream ss;
INIT
{
    zl::Logger::to(ss);
}

TEST(zlog)
{
    zlog 1, 2, 3, 4;
    check("1 2 3 4\n")

    zloga(false) 1, 2, 3, 4;
    check("1 2 3 4")

    zloga(false, false) 1, 2, 3, 4;
    check("1234")

    zlog vector<int> {1, 2, 3};
    check("[1, 2, 3]\n")

    zlog A();
    check("unknown\n")

}

TEST(tuple)
{
    zlog make_tuple(1, 'c');
    check("(1, c)\n")
}

TEST_MAIN
