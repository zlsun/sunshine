#include "../include/zstatic.h"
#include "../include/ztest.h"

TEST(static_if) {
    STATIC_IF(false) {
        EQUAL(1, 2)
    } STATIC_ELIF(true) {
        EQUAL(1, 1)
    } STATIC_ELSE {
        EQUAL(1, 2)
    };
    int i = STATIC_IF(false) {
        return 1;
    } STATIC_ELIF(false) {
        return 2;
    } STATIC_ELSE {
        return 3;
    };
    EQUAL(i, 3)
}

TEST_MAIN
