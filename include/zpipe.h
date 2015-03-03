#ifndef ZPIPE_H
#define ZPIPE_H

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cstring>

#include "zlog.h"
#include "ztraits.h"
#include "zassert.h"

namespace zl {

// ===========================================================================

template <typename Enum>
struct IEnum {
    friend std::ostream& operator << (std::ostream& out, Enum e) {
        if (e.over()) {
            return out << "[]";
        }
        out << "[" << e.current();
        e.advance();
        while (!e.over()) {
            out << ", " << e.current();
            e.advance();
        }
        return out << "]";
    }
};

template <typename Enum, typename Func>
auto operator | (Enum e, Func f)
-> decltype(f(e)) {
    return f(e);
}

// ===========================================================================

template <typename It>
struct StdEnum : IEnum<StdEnum<It>> {
    using ValueT = typename std::iterator_traits<It>::value_type;
    using IterT = It;
    IterT icur;
    IterT iend;
    StdEnum(IterT b, IterT e): icur(b), iend(e) {}
    ValueT current() const {
        return *icur;
    }
    bool over() const {
        return icur == iend;
    }
    void advance() {
        ++icur;
    }
};

template <typename It>
StdEnum<It> ifrom(It start, It end) {
    return StdEnum<It>(start, end);
}
template <typename T>
auto ifrom(const T& t)
-> decltype(ifrom(std::begin(t), std::end(t))) {
    return ifrom(std::begin(t), std::end(t));
}
template <typename T>
auto ifrom(const std::initializer_list<T>& t)
-> decltype(ifrom(std::begin(t), std::end(t))) {
    return ifrom(std::begin(t), std::end(t));
}
StdEnum<const char*> ifrom(const char* s) {
    return ifrom(s, s + std::strlen(s));
}

// ===========================================================================

template <typename T>
struct RepeatEnum : IEnum<RepeatEnum<T>> {
    using ValueT = T;
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

template <typename T>
RepeatEnum<T> irepeat(T t, size_t n = 0) {
    return RepeatEnum<T>(t, n);
}

// ===========================================================================

template <typename T>
struct RangeEnum : IEnum<RangeEnum<T>> {
    using ValueT = T;
    ValueT cur, end, step;
    RangeEnum(const ValueT& b, const ValueT& e, const ValueT& s)
        : cur(b), end(e), step(s) {}
    ValueT current() const {
        return cur;
    }
    bool over() const {
        return cur == end;
    }
    void advance() {
        cur += step;
    }
};

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
    std::vector<typename E::ValueT> operator () (E& e) const {
        std::vector<typename E::ValueT> v;
        while (!e.over()) {
            v.push_back(e.current());
            e.advance();
        }
        return v;
    }
} to_vector;

// ===========================================================================

template <typename E, typename F>
struct SelectEnum: IEnum<SelectEnum<E, F>>, E {
    F f;
    SelectEnum(const E& e, F f): E(e), f(f) {}
    typename function_traits<F>::result_type current() const {
        return f(E::current());
    }
};

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
struct WhereEnum: IEnum<WhereEnum<E, F>>, E {
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
    WhereEnum<E, F> operator () (E& e) const {
        return WhereEnum<E, F>(e, f);
    }
};

template <typename F>
Where<F> iwhere(F f) {
    return Where<F>(f);
}

// ===========================================================================

template <typename F>
struct Aggregate {
    F f;
    Aggregate(F f): f(f) {}
    template <typename E>
    typename E::ValueT operator () (E& e) const {
        typename E::ValueT result;
        if (e.over()) {
            return result;
        }
        result = e.current();
        while (!e.over()) {
            result = f(result, e.current());
            e.advance();
        }
        return result;
    }
};

template <typename F>
Aggregate<F> iaggrerate(F f) {
    return Aggregate<F>(f);
}

// ===========================================================================

template <typename F, typename T>
struct Aggregate2 {
    F f;
    T init;
    Aggregate2(F f, T t): f(f), init(t) {}
    template <typename E>
    T operator () (E& e) const {
        T result = init;
        while (!e.over()) {
            result = f(result, e.current());
            e.advance();
        }
        return result;
    }
};

template <typename F, typename T>
Aggregate2<F, T> iaggrerate2(F f, T init) {
    return Aggregate2<F, T>(f, init);
}

// ===========================================================================

struct Max {
    template <typename U>
    U operator () (const U& u, const U& v) const {
        return v > u ? v : u;
    }
};

auto imax = Aggregate<Max>(Max());

// ===========================================================================

struct Min {
    template <typename U>
    U operator () (const U& u, const U& v) const {
        return v < u ? v : u;
    }
};

auto imin = Aggregate<Min>(Min());

// ===========================================================================

struct Sum {
    template <typename U, typename V>
    U operator () (const U& u, const V& v) const {
        return u + v;
    }
};

template <typename T>
Aggregate2<Sum, T> isum(const T& init) {
    return Aggregate2<Sum, T>(Sum(), init);
}

auto isum() -> decltype(isum(0)) {
    return isum(0);
}

// ===========================================================================

template <typename T>
struct Count {
    T x;
    Count(T x): x(x) {}
    template <typename U, typename V>
    U operator () (const U& u, const V& v) const {
        if (v == x) {
            return u + 1;
        } else {
            return u;
        }
    }
};

template <typename T, typename S = std::size_t>
Aggregate2<Count<T>, S> icount(const T& x, const S& init = 0) {
    return Aggregate2<Count<T>, S>(Count<T>(x), init);
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
Concat<std::string, std::string> iconcat(char c, int n = 1) {
    return iconcat<std::string, std::string>(std::string(n, c));
}
Concat<std::string, std::string> iconcat(const char* s) {
    return iconcat<std::string, std::string>(std::string(s));
}
Concat<std::string, std::string> iconcat() {
    return iconcat("");
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

template <typename M>
class MemberHolder {
protected:
    M m;
public:
    MemberHolder(M m): m(m){}
    M& get() {
        return m;
    };
    const M& get() const {
        return m;
    };
};

template <typename E>
struct ReverseEnum
    : IEnum<ReverseEnum<E>>
    , MemberHolder<std::vector<typename E::ValueT>>
    , StdEnum<typename std::vector<typename E::ValueT>::reverse_iterator>
{
    using VecMem = MemberHolder<std::vector<typename E::ValueT>>;
    using Base = StdEnum<typename std::vector<typename E::ValueT>::reverse_iterator>;
    ReverseEnum(const E& e)
        : VecMem(e | to_vector)
        , Base(VecMem::get().rbegin(), VecMem::get().rend())
    {}
};

struct Reverse {
    template <typename E>
    ReverseEnum<E> operator () (E& e) const {
        return ReverseEnum<E>(e);
    }
} ireverse;

// ===========================================================================

template <typename M>
class SortedMemberHolder: public MemberHolder<M> {
public:
    SortedMemberHolder(M m): MemberHolder<M>(m) {
        std::sort(MemberHolder<M>::get().begin(), MemberHolder<M>::get().end());
    }
    template <typename F>
    SortedMemberHolder(M m, F cmp): MemberHolder<M>(m) {
        std::sort(MemberHolder<M>::get().begin(), MemberHolder<M>::get().end(), cmp);
    }
};

template <typename E>
struct SortEnum
    : IEnum<SortEnum<E>>
    , SortedMemberHolder<std::vector<typename E::ValueT>>
    , StdEnum<typename std::vector<typename E::ValueT>::iterator>
{
    using SortedVecMem = SortedMemberHolder<std::vector<typename E::ValueT>>;
    using Base = StdEnum<typename std::vector<typename E::ValueT>::iterator>;
    SortEnum(const E& e)
        : SortedVecMem(e | to_vector)
        , Base(SortedVecMem::get().begin(), SortedVecMem::get().end())
    {}
    template <typename F>
    SortEnum(const E& e, F cmp)
        : SortedVecMem(e | to_vector, cmp)
        , Base(SortedVecMem::get().begin(), SortedVecMem::get().end())
    {}
};

struct Sort {
    template <typename E>
    SortEnum<E> operator () (E& e) const {
        return SortEnum<E>(e);
    }
} isort;

template <typename F>
struct SortBy {
    F cmp;
    SortBy(F f): cmp(f) {}
    template <typename E>
    SortEnum<E> operator () (E& e) const {
        return SortEnum<E>(e, cmp);
    }
};

template <typename F>
auto isortby(F cmp) -> SortBy<F> {
    return SortBy<F>(cmp);
}

// ===========================================================================

// ===========================================================================

} // namespace zl

#endif // ZPIPE_H
