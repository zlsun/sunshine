#include <vector>
#include <iostream>
#include "../include/zalgo.h"
#include "../include/ztest.h"
using namespace std;

TEST(copy)
    const vector<int> v {1, 3, 5, 2, 4};
    vector<int> v2(5);
    copy(v, v2);
    EQUAL(v2[1], 3)
    copy(vector<int> {1, 2, 5, 3, 4}, v2);
    EQUAL(v2[1], 2)
END

TEST(sort)
    int array[] {1, 3, 5, 2, 4};
    vector<int> v(BEG_END(array));
    sort(array);
    EQUAL(array[4], 5)
    sort(v);
    EQUAL(v[2], 3)
END

TEST(transform)
    int array[] {1, 3, 5, 2, 4};
    transform(array, array, [](int i) {
        return i + 1;
    });
    EQUAL(array[1], 4)
    transform_2(array, array, array, [](int i, int j) {
        return i + j;
    });
    EQUAL(array[1], 8)
    transform_2(vector<int> {1, 2, 5, 3, 4}, array, array, [](int i, int j) {
        return i + j;
    });
    EQUAL(array[1], 10)
END

TEST(relative_access)
    const vector<int> v {1, 3, 5, 2, 4};
    EQUAL(*(v >> 0), 1)
    EQUAL(*(v >> 2), 5)
    EQUAL(*(v << 2), 2)
    EQUAL(*(v << 1), 4)
END
