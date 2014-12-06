#include <iostream>
#include <vector>
#include <iterator>
#include <cstring>
#include <ztraits.h>
#include <zlog.h>

namespace zl {
using namespace std;

// ===========================================================================

template <typename It>
struct Enum {
    It icurrent;
    It iend;
    using T = typename iterator_traits<It>::value_type;

    Enum(It b, It e): icurrent(b), iend(e) {}
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

template <typename Enum, typename Func>
auto operator | (Enum e, Func f)
    -> decltype(f(e))
{
    return f(e);
}

// ===========================================================================

template <typename T>
Enum<T> ifrom(T start, T end) {
    return Enum<T>(start, end);
}
template <typename T>
auto ifrom(const T& t)
    -> decltype(ifrom(begin(t), end(t)))
{
    return ifrom(begin(t), end(t));
}
template <typename T>
auto ifrom(const initializer_list<T>& t)
    -> decltype(ifrom(begin(t), end(t)))
{
    return ifrom(begin(t), end(t));
}
Enum<const char*> ifrom(const char* s) {
    return ifrom(s, s + strlen(s));
}

// ===========================================================================

struct ToVector {
    template <typename E>
    vector<typename E::T> operator () (E& e) const
    {
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
    auto current() const
        -> typename function_traits<F>::result_type
    {
        return f(E::current());
    }
};
template <typename F>
struct Select {
    F f;
    Select(F f): f(f) {}
    template <typename E>
    auto operator () (E& e) const
        -> SelectEnum<E, F>
    {
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
template <typename F>
struct Where {
    F f;
    Where(F f): f(f) {}
    template <typename E>
    auto operator () (E& e) const
        -> WhereEnum<E, F>
    {
        return WhereEnum<E, F>(e, f);
    }
};
template <typename F>
Where<F> iwhere(F f) {
    return Where<F>(f);
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

template <typename T, typename S = size_t>
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
Concat<string, string> iconcat(char c) {
    return iconcat<string, string>(string(1, c));
}
Concat<string, string> iconcat(const char* s) {
    return iconcat<string, string>(string(s));
}
Concat<string, string> iconcat() {
    return iconcat("");
}

// ===========================================================================

} // namespace zl
