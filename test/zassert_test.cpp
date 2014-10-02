#include "../include/ztest.h"
#include "../include/zassert.h"
using namespace std;

test_begin(zassert);
    int i = 1, j = 2;
    SMART_ASSERT_NOABORT(i > j)(i)(j);
    test_succeed();
test_end();

test_main();
