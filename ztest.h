#ifndef ZTEST_H
#define ZTEST_H

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdio>

static std::vector<std::pair<std::string, bool(*)()>> __tests;

#define test_init() int __passed = 0, __failed = 0

#define test_succeed() __failed = 0

#define test_failed(msg) (++__failed &&                                                         \
    std::cout << "(line " << __LINE__ << ") failure #" << __failed << ": "                      \
              << msg << std::endl)

#define test_true(con) ((con) ? (++__passed) : (++__failed &&                                   \
    std::cout << "(line " << __LINE__ << ") failure #" << __failed << ": "                      \
              << #con << std::endl))

#define test_false(con) test_true(!(con))

#define test_equal(a, b) ((a) == (b) ? (++__passed) : (++__failed &&                            \
    std::cout << "(line " << __LINE__ << ") failure #" << __failed << ": "                      \
              << #a " == " #b " [with (" #a ") = " << (a) << ", (" #b ") = " << (b) << "]"      \
              << std::endl))

#define test_report() (std::cout << "\tPassed: "<< __passed << ", Failed: " << __failed << std::endl)

#define test_begin(name)                                        \
    bool name();                                                \
    struct __struct_##name {                                    \
        __struct_##name() {                                     \
            __tests.push_back({#name, name});                   \
        }                                                       \
    } __struct_##name;                                          \
    bool name() {                                               \
        std::cout << "=======================\n"                \
                  << "Test: " #name << std::endl;               \
        test_init()

#define test_end()                                              \
        test_report();                                          \
        return __failed != 0;                                   \
    } //

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
    cout << "\n***********************" << endl;
    cout << passed_tests.size() << " tests passed:" << endl;
    for (auto p : passed_tests) {
        cout << "\t" << p << endl;
    }
    cout << failed_tests.size() << " tests failed:" << endl;
    for (auto p : failed_tests) {
        cout << "\t" << p << endl;
    }
}

#define test_main()                             \
    int main(int argc, char const *argv[]) {    \
        test_run();                             \
        std::getchar();                         \
        return 0;                               \
    } //

#endif // ZTEST_H