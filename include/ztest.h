#ifndef ZTEST_H
#define ZTEST_H

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdio>

static std::vector<std::pair<std::string, bool(*)()>> __tests;

#define test_init() int __passed = 0, __failed = 0;

#define test_judge_true(msg) ++__passed;

#define test_judge_false(msg) ++__failed; std::cout << msg << std::endl;

#define test_judge(con, true_msg, false_msg)    \
    if (con) {                                  \
        test_judge_true(true_msg)               \
    } else {                                    \
        test_judge_false(false_msg)             \
    }

#define test_succeed() __failed = 0;

#define test_failed(msg) test_judge_false("(line " << __LINE__ << ") failure #" << __failed << ": " << msg)

#define test_true(con) test_judge(con, "", "(line " << __LINE__ << ") failure #" << __failed << ": " << #con)

#define test_false(con) test_true(!(con))

#define test_equal(a, b) test_judge((a) == (b), "",                                 \
    "(line " << __LINE__ << ") failure #" << __failed << ":\n"                       \
    << #a " == " #b ", with\n\t" #a " = " << (a) << "\n\t" #b " = " << (b) << "")

#define test_report() std::cout << "Passed: "<< __passed << ", Failed: " << __failed << std::endl;

#define test_begin(name)                                        \
    bool __test_##name();                                       \
    struct __struct_##name {                                    \
        __struct_##name() {                                     \
            __tests.push_back({#name, __test_##name});          \
        }                                                       \
    } __struct_##name;                                          \
    bool __test_##name() {                                      \
        std::cout << "=======================\n"                \
                  << "Test: " #name << std::endl;               \
        test_init()

#define test_end()                                              \
        test_report();                                          \
        return __failed != 0;                                   \
    }

void test_run() {
    using namespace std;
    vector<string> passed_tests, failed_tests;
    for (auto p : __tests) {
        if ((p.second)()) {
            failed_tests.push_back(p.first);
        } else {
            passed_tests.push_back(p.first);
        }
    }
    cout << "***********************" << endl;
    cout << passed_tests.size() << " tests passed:" << endl;
    for (auto p : passed_tests) {
        cout << "  " << p;
    }
    cout << endl;
    cout << failed_tests.size() << " tests failed:" << endl;
    for (auto p : failed_tests) {
        cout << "  " << p << endl;
    }
    cout << endl;
}

#define test_main()                             \
    int main(int argc, char const *argv[]) {    \
        test_run();                             \
        std::getchar();                         \
        return 0;                               \
    }

#endif // ZTEST_H