#ifndef ZTEST_H
#define ZTEST_H

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdio>

extern std::vector<std::pair<std::string, bool(*)()>> __tests;

#define JUDGE_TRUE(msg) ++__passed;

#define JUDGE_FALSE(msg) ++__failed; std::cout << msg << std::endl;

#define JUDGE(con, true_msg, false_msg) \
    if (con) {                          \
        JUDGE_TRUE(true_msg)            \
    } else {                            \
        JUDGE_FALSE(false_msg)          \
    }

#define EQUAL(a, b) JUDGE((a) == (b), "", "(line " << __LINE__ << ") failure #" << __failed << ":\n" \
    << #a " == " #b ", with\n\t" #a " : " << (a) << "\n\t" #b " : " << (b) << "")

#define SUCCEED __failed = 0;

#define FAILED(msg) JUDGE_FALSE("(line " << __LINE__ << ") failure #" << __failed << ": " << msg);

#define TEST(name)                                              \
    bool __test_##name();                                       \
    struct __struct_##name {                                    \
        __struct_##name() {                                     \
            __tests.push_back({#name, __test_##name});          \
        }                                                       \
    } __struct_##name;                                          \
    bool __test_##name() {                                      \
        int __passed = 0, __failed = 0;                         \
        std::cout << "=======================\n"                \
                  << "Test: " #name << std::endl;               \

#define END                                                     \
        std::cout << "Passed: "<< __passed << ", Failed: "      \
                  << __failed << std::endl;                     \
        return __failed != 0;                                   \
    }

#endif // ZTEST_H
