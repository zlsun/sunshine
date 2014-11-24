#include "../include/ztraits.h"
#include "../include/ztest.h"

using namespace zl;

test_begin(ztraits_test);
    auto f1 = [](int a) {};
    auto f2 = [](int a, int b) {};
    test_equal(is_unary_function<decltype(f1)>::value, true);
    test_equal(is_unary_function<decltype(f2)>::value, false);
    test_equal(is_binary_function<decltype(f2)>::value, true);
test_end();

test_main();
