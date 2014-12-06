#include <vector>
#include <iostream>
#include "../include/zalgo.h"
#include "../include/ztest.h"
using namespace std;

test_begin(copy);
    const vector<int> v {1, 3, 5, 2, 4};
    vector<int> v2(5);
    copy(v, v2);
    test_equal(v2[1], 3);
    copy(vector<int> {1, 2, 5, 3, 4}, v2);
    test_equal(v2[1], 2);
test_end();

test_begin(sort);
    int array[] {1, 3, 5, 2, 4};
    vector<int> v(BEG_END(array));
    sort(array);
    test_equal(array[4], 5);
    sort(v);
    test_equal(v[2], 3);
test_end();

test_begin(transform);
    int array[] {1, 3, 5, 2, 4};
    transform(array, array, [](int i) {
        return i + 1;
    });
    test_equal(array[1], 4);
    transform_2(array, array, array, [](int i, int j) {
        return i + j;
    });
    test_equal(array[1], 8);
    transform_2(vector<int> {1, 2, 5, 3, 4}, array, array, [](int i, int j) {
        return i + j;
    });
    test_equal(array[1], 10);
test_end();

test_begin(relative_access);
    const vector<int> v {1, 3, 5, 2, 4};
    test_equal(*(v >> 0), 1);
    test_equal(*(v >> 2), 5);
    test_equal(*(v << 2), 2);
    test_equal(*(v << 1), 4);
test_end();

test_main();