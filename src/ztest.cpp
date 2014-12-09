#include "../include/ztest.h"

std::vector<std::pair<std::string, bool(*)()>> __tests;

int main(int argc, char const *argv[]) {
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
    return 0;
}