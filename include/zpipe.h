#ifndef ZPIPE_H
#define ZPIPE_H

#include <iostream>
#include <vector>
#include <iterator>
#include <cstring>

#include "ztraits.h"
#include "zlog.h"
#include "zassert.h"

namespace zl {

using namespace std;

// ===========================================================================

template <typename T>
struct IEnum {
    T current() const;
    bool over() const;
    void advance() const;
};

#define DEFINE_PRINT_FUNCTION(Enum)                             \
    template <typename... Args>                                 \
    ostream& operator << (ostream& out, Enum<Args...> e) {      \
        if (e.over()) {                                         \
            return out << "[]";                                 \
        }                                                       \
        out << "[" << e.current();                              \
        e.advance();                                            \
        while (!e.over()) {                                     \
            out << ", " << e.current();                         \
            e.advance();                                        \
        }                                                       \
        return out << "]";                                      \
    }

template <typename Enum, typename Func>
auto operator | (Enum e, Func f)
-> decltype(f(e)) {
    return f(e);
}

// ===========================================================================

template <typename It>
struct StdEnum : IEnum<typename iterator_traits<It>::value_type> {
    It icurrent;
    It iend;
    using T = typename iterator_traits<It>::value_type;
    StdEnum(It b, It e): icurrent(b), iend(e) {}
    T current() const {
        return *icurrent;
    }
    bool over() const {
        return icurrent == iend;
    }
    void advance() {
        ++icurrent;
    }
};
DEFINE_PRINT_FUNCTION(StdEnum)

template <typename It>
StdEnum<It> ifrom(It start, It end) {
    return StdEnum<It>(start, end);
}
template <typename T>
auto ifrom(const T& t)
-> decltype(ifrom(begin(t), end(t))) {
    return ifrom(begin(t), end(t));
}
template <typename T>
auto ifrom(const initializer_list<T>& t)
-> decltype(ifrom(begin(t), end(t))) {
    return ifrom(begin(t), end(t));
}
StdEnum<const char*> ifrom(const char* s) {
    return ifrom(s, s + strlen(s));
}

// ===========================================================================

template <typename T>
struct RepeatEnum : IEnum<T> {
    T x;
    size_t n, i = 0;
    RepeatEnum(const T& x, size_t n): x(x), n(n) {}
    T current() const {
        return x;
    }
    bool over() const {
        return i == n;
    }
    void advance() {
        ++i;
    }
};
DEFINE_PRINT_FUNCTION(RepeatEnum)

template <typename T>
RepeatEnum<T> irepeat(T t, size_t n = 0) {
    return RepeatEnum<T>(t, n);
}

// ===========================================================================

template <typename T>
struct RangeEnum : IEnum<T> {
    T cur, end, step;
    RangeEnum(const T& b, const T& e, const T& s): cur(b), end(e), step(s) {}
    T current() const {
        return cur;
    }
    bool over() const {
        return cur == end;
    }
    void advance() {
        cur += step;
    }
};
DEFINE_PRINT_FUNCTION(RangeEnum)

template <typename T>
RangeEnum<T> irange(const T& b, const T& e, const T& s) {
    return RangeEnum<T>(b, e, s);
}
RangeEnum<int> irange(int b, int e, int s) {
    SMART_ASSERT((s > 0 && b <= e) || (s < 0 && b >= e))(b)(e)(s);
    return RangeEnum<int>(b, e + (e - b) % s, s);
}
RangeEnum<int> irange(int b, int e) {
    return RangeEnum<int>(b, e, (b < e ? 1 : -1));
}
RangeEnum<int> irange(int e) {
    return irange(0, e);
}

// ===========================================================================

struct ToVector {
    template <typename E>
    vector<typename E::T> operator () (E& e) const {
        vector<typename E::T> v;
        while (!e.over()) {
            v.push_back(e.current());
            e.advance();
        }
        return v;
    }
} to_vector;

// ===========================================================================

template <typename E, typename F>
struct SelectEnum: E {
    F f;
    SelectEnum(const E& e, F f): E(e), f(f) {}
    typename function_traits<F>::result_type current() const {
        return f(E::current());
    }
};
DEFINE_PRINT_FUNCTION(SelectEnum)

template <typename F>
struct Select {
    F f;
    Select(F f): f(f) {}
    template <typename E>
    SelectEnum<E, F> operator () (E& e) const {
        return SelectEnum<E, F>(e, f);
    }
};
template <typename F>
Select<F> iselect(F f) {
    return Select<F>(f);
}

// ===========================================================================

template <typename E, typename F>
struct WhereEnum: E {
    F f;
    WhereEnum(const E& e, F f): E(e), f(f) {}
    void advance() {
        do {
            E::advance();
        } while (!E::over() && !f(E::current()));
    }
};
DEFINE_PRINT_FUNCTION(WhereEnum)

template <typename F>
struct Where {
    F f;
    Where(F f): f(f) {}
    template <typename E>
    WhereEnum<E, F> operator () (E& e) const {
        return WhereEnum<E, F>(e, f);
    }
};
template <typename F>
Where<F> iwhere(F f) {
    return Where<F>(f);
}

// ===========================================================================

template <typename T, typename R>
struct Concat {
    T split;
    Concat(const T& s): split(s) {}
    template <typename E>
    R operator () (E& e) const {
        R result;
        if (!e.over()) {
            result += e.current();
            e.advance();
        }
        while (!e.over()) {
            result += split;
            result += e.current();
            e.advance();
        }
        return result;
    }
};
template <typename R, typename T>
Concat<T, R> iconcat(const T& t) {
    return Concat<T, R>(t);
}
Concat<string, string> iconcat(char c, int n = 1) {
    return iconcat<string, string>(string(n, c));
}
Concat<string, string> iconcat(const char* s) {
    return iconcat<string, string>(string(s));
}
Concat<string, string> iconcat() {
    return iconcat("");
}

// ===========================================================================

template <typename T>
struct Sum {
    T init;
    Sum(const T& i): init(i) {}
    template <typename E>
    T operator () (E& e) const {
        T sum(init);
        while (!e.over()) {
            sum += e.current();
            e.advance();
        }
        return sum;
    }
};
template <typename T>
Sum<T> isum(const T& init) {
    return Sum<T>(init);
}
Sum<int> isum() {
    return isum(0);
}

// ===========================================================================

template <typename T, typename S>
struct Count {
    T x;
    Count(const T& x): x(x) {}
    template <typename E>
    S operator () (E& e) const {
        S count = 0;
        while (!e.over()) {
            if (e.current() == x) {
                ++count;
            }
            e.advance();
        }
        return count;
    }
};
template <typename T, typename S = size_t>
Count<T, S> icount(const T& x) {
    return Count<T, S>(x);
}

// ===========================================================================

template <typename F>
struct All {
    F f;
    All(const F& f): f(f) {}
    template <typename E>
    bool operator () (E& e) const {
        while (!e.over()) {
            if (!f(e.current())) {
                return false;
            }
            e.advance();
        }
        return true;
    }
};
template <typename F>
All<F> iall(const F& f) {
    return All<F>(f);
}

// ===========================================================================

template <typename F>
struct Any {
    F f;
    Any(const F& f): f(f) {}
    template <typename E>
    bool operator () (E& e) const {
        while (!e.over()) {
            if (f(e.current())) {
                return true;
            }
            e.advance();
        }
        return false;
    }
};
template <typename F>
Any<F> iany(const F& f) {
    return Any<F>(f);
}

// ===========================================================================

#undef DEFINE_PRINT_FUNCTION

} // namespace zl

#endif // ZPIPE_H
