#ifndef ZUTILS_H
#define ZUTILS_H

#include "zcommon.h"

NS_ZL_BEGIN

template <class T>
struct ImplementRelationalOperators {
    friend bool operator <= (const T& a, const T& b) {
        return a < b || a == b;
    }
    friend bool operator > (const T& a, const T& b) {
        return !(a <= b);
    }
    friend bool operator != (const T& a, const T& b) {
        return !(a == b);
    }
    friend bool operator >= (const T& a, const T& b) {
        return !(a < b);
    }
};

NS_ZL_END

#endif // ZUTILS_H
