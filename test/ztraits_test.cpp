#include "../include/ztraits.h"
#include "../include/ztest.h"

using namespace zl;

TEST(ztraits)
    auto f1 = [](int a) {};
    auto f2 = [](int a, int b) {};
    EQUAL(is_unary_function<decltype(f1)>::value, true)
    EQUAL(is_unary_function<decltype(f2)>::value, false)
    EQUAL(is_binary_function<decltype(f2)>::value, true)
END
