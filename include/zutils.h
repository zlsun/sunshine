#ifndef ZUTILS_H
#define ZUTILS_H

namespace zl {

template <class T>
struct implement_relational_operators {
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

}

#endif // ZUTILS_H
