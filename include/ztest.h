#ifndef ZTEST_H
#define ZTEST_H

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdio>

std::vector<std::pair<std::string, bool(*)()>> __tests;

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

#define END                                                     \
        return __passed != 0 && __failed != 0;                  \
    }

#define TEST_MAIN                                                   \
    int main(int argc, char const *argv[]) {                        \
        using namespace std;                                        \
        vector<string> passed_tests, failed_tests;                  \
        cout << "[" __FILE__ "]" << endl;                           \
        for (auto p : __tests) {                                    \
            if ((p.second)()) {                                     \
                failed_tests.push_back(p.first);                    \
            } else {                                                \
                passed_tests.push_back(p.first);                    \
            }                                                       \
        }                                                           \
        cout << passed_tests.size() << " tests passed:";            \
        for (auto p : passed_tests) {                               \
            cout << "  " << p;                                      \
        }                                                           \
        cout << endl;                                               \
        cout << failed_tests.size() << " tests failed:";            \
        for (auto p : failed_tests) {                               \
            cout << "  " << p << endl;                              \
        }                                                           \
        cout << endl << endl;                                       \
        return 0;                                                   \
    }

#endif // ZTEST_H
