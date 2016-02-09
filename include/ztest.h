#ifndef ZTEST_H
#define ZTEST_H

#include <iostream>
#include <vector>

#include "zcommon.h"
#include "zinit.h"

NS_ZL_BEGIN

std::vector<std::pair<
    std::string,
    void(*)(int&, int&)
>> _ztest_tests;

NS_ZL_END

#define _ZTEST_JUDGE(con, passed_msg, failed_msg) \
    if (con) {                                    \
        ++_ztest_passed;                          \
        std::cout << passed_msg;                  \
    } else {                                      \
        ++_ztest_failed;                          \
        std::cout << failed_msg;                  \
    }

#define _ZTEST_FAILED_MSG_PREFIX "#" << _ztest_failed << "(" << __LINE__ << "): "

#define CHECK(check) _ZTEST_JUDGE(check, "", _ZTEST_FAILED_MSG_PREFIX << #check << "\n")

#define EQUAL(a, b) _ZTEST_JUDGE((a) == (b), "",  \
    _ZTEST_FAILED_MSG_PREFIX << #a" == "#b"\n" << \
    "\t"#a" = " << (a) << "\n\t"#b" = " << (b) << "\n")

#define FAILED(msg) _ZTEST_JUDGE(false, "", _ZTEST_FAILED_MSG_PREFIX << msg << "\n")

#define TEST(name)                                                  \
    void _ztest_test_##name(int&, int&);                            \
    INIT                                                            \
    {                                                               \
        zl::_ztest_tests.push_back({#name, _ztest_test_##name});    \
    }                                                               \
    void _ztest_test_##name(int& _ztest_passed, int& _ztest_failed) \

#define TEST_MAIN                                  \
    int main(int argc, char const *argv[])         \
    {                                              \
        using namespace std;                       \
        vector<string> passed_tests, failed_tests; \
        cout << "\e[1;31m";                        \
        cout << "[" __FILE__ "]" << endl;          \
        cout << "\e[0m";                           \
        for (auto p : zl::_ztest_tests) {          \
            int passed = 0, failed = 0;            \
            p.second(passed, failed);              \
            if (failed) {                          \
                cout << "\e[31m";                  \
                cout << " ❌ " << p.first << endl;  \
                cout << "\e[0m";                   \
                failed_tests.push_back(p.first);   \
            } else {                               \
                cout << "\e[32m";                  \
                cout << " ✓ " << p.first << endl;  \
                cout << "\e[0m";                   \
                passed_tests.push_back(p.first);   \
            }                                      \
        }                                          \
        return 0;                                  \
    }

#endif // ZTEST_H
