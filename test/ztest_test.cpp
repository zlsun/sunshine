#include "../ztest.h"

test_begin(ztest_test);
    test_true(1 == 1);
    test_equal(__failed, 0);
    test_true(1 == 2);
    test_equal(__failed, 1);
    test_false(1 == 2);
    test_equal(__failed, 1);
    test_false(1 == 1);
    test_equal(__failed, 2);
    test_equal(1 + 1, 1 + 2);
    test_equal(__failed, 3);
    if (__failed == 3) {
        test_succeed();
    }
test_end();

test_main();
