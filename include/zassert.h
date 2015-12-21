// clone from https://github.com/hjcapple/clover/blob/master/clover/Debug.h

#ifndef ZASSERT_H
#define ZASSERT_H

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "zcommon.h"

NS_ZL_BEGIN

class NullAssert
{
public:
    char __ZASSERT_A;
    char __ZASSERT_B;
};

class Assert : public NullAssert
{
public:
    Assert(bool needAbort) : _hasPrintValue(false), _abort(needAbort)
    {
    }

    ~Assert()
    {
        std::cout << "\n";
        if (_abort) {
            abort();
        }
    }
    Assert& printContext(const char* file, const char* function, int line)
    {
        std::cout << "File: " << file << " "
                  << "Line: " << line << "\n"
                  << "Function: " << function << "\n";
        return *this;
    }
    template <typename T>
    Assert& printValue(const char* key, const T& value)
    {
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

inline Assert makeAssert(bool needAbort, const char* msg)
{
    if (msg && *msg) {
        std::cout << "Assert failed: " << msg << "\n";
    }
    return Assert(needAbort);
}

#define __ZASSERT_A(x) __ZASSERT_OP(x, B)
#define __ZASSERT_B(x) __ZASSERT_OP(x, A)

#ifdef NDEBUG
# define __ZASSERT_OP(x, next) __ZASSERT_##next
# define ZASSERT(expr) zl::NullAssert().__ZASSERT_A
# define ZASSERT_NOCONTEXT(expr) zl::NullAssert().__ZASSERT_A
# define ZASSERT_NOABORT(expr) zl::NullAssert().__ZASSERT_A
# define ZASSERT_NOCONTEXT_NOABORT(expr) zl::NullAssert().__ZASSERT_A
#else
# define __ZASSERT_OP(x, next) printValue(#x, (x)).__ZASSERT_##next
# define ZASSERT(expr) \
    if (!(expr)) zl::makeAssert(true, #expr).printContext(__FILE__, __FUNCTION__, __LINE__).__ZASSERT_A
# define ZASSERT_NOCONTEXT(expr) \
    if (!(expr)) zl::makeAssert(true, #expr).__ZASSERT_A
# define ZASSERT_NOABORT(expr) \
    if (!(expr)) zl::makeAssert(false, #expr).printContext(__FILE__, __FUNCTION__, __LINE__).__ZASSERT_A
# define ZASSERT_NOCONTEXT_NOABORT(expr) \
    if (!(expr)) zl::makeAssert(false, #expr).__ZASSERT_A
#endif

NS_ZL_END

#endif // ZASSERT_H
