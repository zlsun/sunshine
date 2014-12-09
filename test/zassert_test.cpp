#include "../include/ztest.h"
#include "../include/zassert.h"
using namespace std;

TEST(zassert)
    int i = 1, j = 2;
    SMART_ASSERT_NOABORT(i > j)(i)(j);
    SUCCEED
END
