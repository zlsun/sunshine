#include <vector>
#include <iostream>

#include "../include/zalgo.h"
#include "../include/ztest.h"

using namespace std;

using Vi = vector<int>;

TEST(copy)
{
    const Vi v {1, 3, 5, 2, 4};
    Vi v2(5);
    copy(v, v2);
    EQUAL(v2[1], 3)
    copy(Vi {1, 2, 5, 3, 4}, v2);
    EQUAL(v2[1], 2)
}

TEST(sort)
{
    int array[] {1, 3, 5, 2, 4};
    const int N = sizeof(array) / sizeof(*array);
    Vi v(array, array + N);
    sort(array);
    EQUAL(array[4], 5)
    sort(v);
    EQUAL(v[2], 3)
}

TEST(transform)
{
    int array[] {1, 3, 5, 2, 4};
    transform(array, array, [](int i) {
        return i + 1;
    });
    EQUAL(array[1], 4)
    transform2(array, array, array, [](int i, int j) {
        return i + j;
    });
    EQUAL(array[1], 8)
    transform2(Vi {1, 2, 5, 3, 4}, array, array, [](int i, int j) {
        return i + j;
    });
    EQUAL(array[1], 10)
}

TEST(relative_access)
{
    const Vi v {1, 3, 5, 2, 4};
    EQUAL(*(v >> 0), 1)
    EQUAL(*(v >> 2), 5)
    EQUAL(*(v << 2), 2)
    EQUAL(*(v << 1), 4)
    EQUAL(*(Vi {1, 2, 5, 3, 4} << 1), 4)
}

TEST_MAIN
