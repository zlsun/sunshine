// clone from https://github.com/hjcapple/clover/blob/master/clover/Debug.h

#ifndef ZREQUIRE_H
#define ZREQUIRE_H

#include <iostream>
#include <fstream>
#include <cstdlib>

namespace zlsun {

class NullAssert {
public:
    char __SMART_ASSERT_A;
    char __SMART_ASSERT_B;
};

class Assert : public NullAssert {
public:
    Assert(bool needAbort) : _hasPrintValue(false), _abort(needAbort) {
    }

    ~Assert() {
        std::cout << "\n";
        if (_abort) {
            abort();
        }
    }
    Assert& printContext(const char* file, const char* function, int line) {
        std::cout << "File: " << file << " "
                  << "Line: " << line << "\n"
                  << "Function: " << function << "\n";
        return *this;
    }
    template <typename T>
    Assert& printValue(const char* key, const T& value) {
        if (!_hasPrintValue) {
            std::cout << "Context Variables: \n";
            _hasPrintValue = true;
        }
        if (*key == '\"') {
            std::cout << "\t" << value << "\n";
        } else {
            std::cout << "\t" << key << ": " << value << "\n";
        }
        return *this;
    }
private:
    bool _hasPrintValue;
    bool _abort;
};

inline Assert makeAssert(bool needAbort, const char* msg) {
    if (msg && *msg) {
        std::cout << "Assert failed: " << msg << "\n";
    }
    return Assert(needAbort);
}

#define __SMART_ASSERT_A(x) __SMART_ASSERT_OP(x, B)
#define __SMART_ASSERT_B(x) __SMART_ASSERT_OP(x, A)

#ifdef NDEBUG
# define __SMART_ASSERT_OP(x, next) __SMART_ASSERT_##next
# define SMART_ASSERT(expr) zlsun::NullAssert().__SMART_ASSERT_A
# define SMART_ASSERT_NOABORT(expr) zlsun::NullAssert().__SMART_ASSERT_A
#else
# define __SMART_ASSERT_OP(x, next) printValue(#x, (x)).__SMART_ASSERT_##next
# define SMART_ASSERT(expr)  \
    if (!(expr))             \
        zlsun::makeAssert(true, #expr).printContext(__FILE__, __FUNCTION__, __LINE__).__SMART_ASSERT_A
# define SMART_ASSERT_NOABORT(expr)  \
    if (!(expr))             \
        zlsun::makeAssert(false, #expr).printContext(__FILE__, __FUNCTION__, __LINE__).__SMART_ASSERT_A
#endif

} // namespace zlsun

#endif // ZREQUIRE_H