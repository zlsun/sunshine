#include "ztest.h"
#include "zsignal.h"
using namespace zl;

void inc(int& i) {
    ++i;
}

void dec(int& i) {
    --i;
}

Signal<void(int&)> void_signal;

bool ok() {
    return true;
}
bool bad() {
    return false;
}

Signal<bool()> ok_signal;

TEST(connect)
{
    int i = 0;
    void_signal << inc;
    void_signal(i);
    EQUAL(i, 1);
    i = 0;
    void_signal << dec;
    void_signal(i);
    EQUAL(i, 0);

    ok_signal << ok << bad;
    bool o = ok_signal();
    EQUAL(o, false);
}

TEST(disconnect)
{
    int i = 0;
    void_signal >> inc;
    void_signal(i);
    EQUAL(i, -1);
    i = 0;
    void_signal >> dec;
    void_signal(i);
    EQUAL(i, 0);

    ok_signal >> bad;
    bool o = ok_signal();
    EQUAL(o, true);
}

TEST(disconnect_all)
{
    void_signal << inc;
    void_signal.disconnect_all();
    int i = 0;
    void_signal(i);
    EQUAL(i, 0);
}

TEST_MAIN
