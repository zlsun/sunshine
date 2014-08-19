#include <vector>
#include <iostream>
#include <zalgo.h>
#include <ztest.h>
using namespace std;

test_begin(copy);
    const vector<int> v {1, 3, 5, 2, 4};
    vector<int> v2(5);
    copy(v, v2);
    copy(vector<int> {1, 3, 5, 2, 4}, v2);
    test_equal(v2[1], 3);
test_end();

test_begin(sort);
    int array[] {1, 3, 5, 2, 4};
    vector<int> v {begin(array), end(array)};
    sort(array);
    test_equal(array[4], 5);
    sort(v);
    test_equal(v[2], 3);
test_end();

test_begin(relative_operator);
    const vector<int> v {1, 3, 5, 2, 4};
    test_equal(*(v >> 0), 1);
    test_equal(*(v >> 2), 5);
    test_equal(*(v << 2), 2);
    test_equal(*(v << 1), 4);
    const vector<int> v2 {};
    test_true(*(v >> 0));
test_end();

test_main();