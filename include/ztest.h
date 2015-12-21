#ifndef ZTEST_H
#define ZTEST_H

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdio>

#include "zcommon.h"

NS_ZL_BEGIN

std::vector<std::pair<std::string, void(*)(int&, int&)>> __tests;

#define JUDGE_TRUE(msg) ++__passed;

#define JUDGE_FALSE(msg) ++__failed; std::cout << msg << std::endl;

#define JUDGE(con, true_msg, false_msg) \
    if (con) {                          \
        JUDGE_TRUE(true_msg)            \
    } else {                            \
        JUDGE_FALSE(false_msg)          \
    }

#define EQUAL(a, b) JUDGE((a) == (b), "",                                       \
    "(line " << __LINE__ << ") failure #" << __failed << ":\n" <<               \
    #a " == " #b ", with\n\t" #a " : " << (a) << "\n\t" #b " : " << (b) << "")

#define TRUE(s) EQUAL(s, true)

#define FALSE(s) EQUAL(s, false)

#define SUCCEED __failed = 0;

#define FAILED(msg) JUDGE_FALSE("(line " << __LINE__ << ") failure #" << __failed << ": " << msg);

#define TEST(name)                                         \
    void __test_##name(int&, int&);                        \
    struct __struct_##name                                 \
    {                                                      \
        __struct_##name()                                  \
        {                                                  \
            zl::__tests.push_back({#name, __test_##name}); \
        }                                                  \
    } __struct_##name;                                     \
    void __test_##name(int& __passed, int& __failed)       \


#define TEST_MAIN                                        \
    int main(int argc, char const *argv[])               \
    {                                                    \
        using namespace std;                             \
        vector<string> passed_tests, failed_tests;       \
        cout << "[" __FILE__ "]" << endl;                \
        for (auto p : zl::__tests) {                     \
            int passed = 0, failed = 0;                  \
            p.second(passed, failed);                    \
            if (failed) {                                \
                failed_tests.push_back(p.first);         \
            } else {                                     \
                passed_tests.push_back(p.first);         \
            }                                            \
        }                                                \
        cout << passed_tests.size() << " tests passed:"; \
        for (auto p : passed_tests) {                    \
            cout << "  " << p;                           \
        }                                                \
        cout << endl;                                    \
        cout << failed_tests.size() << " tests failed:"; \
        for (auto p : failed_tests) {                    \
            cout << "  " << p << endl;                   \
        }                                                \
        cout << endl << endl;                            \
        return 0;                                        \
    }

NS_ZL_END

#endif // ZTEST_H
