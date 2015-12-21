#ifndef ZPIPE_H
#define ZPIPE_H

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cstring>
#include <cassert>

#include "zcommon.h"
#include "zlog.h"
#include "ztraits.h"

NS_ZL_BEGIN

// ===========================================================================

template <typename Enum>
struct IEnum {
    friend Enum& begin(Enum& e) {
        return e;
    }
    friend Enum& end(Enum& e) {
        return e;
    }
    friend auto operator * (Enum& e) {
        return e.current();
    }
    friend Enum& operator ++ (Enum& e) {
        e.advance();
        return e;
    }
    friend bool operator != (Enum& a, Enum& b) {
        return !a.over();
    }
    friend bool operator ! (Enum& a) {
        return a.over();
    }
    friend std::ostream& operator << (std::ostream& out, Enum e) {
        if (!e) {
            return out << "[]";
        }
        out << "[" << *e;
        ++e;
        for (auto i : e) {
            out << ", " << i;
        }
        return out << "]";
    }
};

template <typename Func>
struct IFunc {
    template <typename Enum>
    auto operator () (Enum e) {
        return Func::apply(e);
    }
};

template <typename Enum, typename Func>
auto operator | (Enum e, Func f) {
    return f(e);
}

// ===========================================================================

template <typename It>
struct StdEnum : IEnum<StdEnum<It>> {
    using ValueT = typename std::iterator_traits<It>::value_type;
    It icur;
    It iend;
    StdEnum(It b, It e): icur(b), iend(e) {}
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
auto ifrom(It start, It end) {
    return StdEnum<It>(start, end);
}
template <typename T>
auto ifrom(const T& t) {
    return ifrom(std::begin(t), std::end(t));
}
template <typename T>
auto ifrom(const std::initializer_list<T>& t) {
    return ifrom(std::begin(t), std::end(t));
}
auto ifrom(const char* s) {
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
    ValueT icur, iend, istep;
    RangeEnum(const ValueT& b, const ValueT& e, const ValueT& s)
        : icur(b), iend(e), istep(s) {}
    ValueT current() const {
        return icur;
    }
    bool over() const {
        return icur == iend;
    }
    void advance() {
        icur += istep;
    }
};

template <typename T>
auto irange(const T& b, const T& e, const T& s) {
    return RangeEnum<T>(b, e, s);
}
auto irange(int b, int e, int s) {
    return RangeEnum<int>(b, e + (e - b) % s, s);
}
auto irange(int b, int e) {
    return RangeEnum<int>(b, e, (b < e ? 1 : -1));
}
auto irange(int e) {
    return irange(0, e);
}

// ===========================================================================

struct ToVector : IFunc<ToVector> {
    template <typename E>
    auto operator () (E& e) const {
        std::vector<typename E::ValueT> v;
        for (auto i : e) {
            v.push_back(i);
        }
        return v;
    }
} to_vector;

// ===========================================================================

template <typename E, typename F>
struct SelectEnum: IEnum<SelectEnum<E, F>>, E {
    F f;
    SelectEnum(const E& e, F f): E(e), f(f) {}
    auto current() const {
        return f(E::current());
    }
};

template <typename F>
struct Select {
    F f;
    Select(F f): f(f) {}
    template <typename E>
    auto operator () (E& e) const {
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
    auto operator () (E& e) const {
        auto result = typename E::ValueT();
        if (!e) {
            return result;
        }
        result = e.current();
        e.advance();
        for (auto i : e) {
            result = f(result, i);
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
struct AggregateInit {
    F f;
    T init;
    AggregateInit(F f, T t): f(f), init(t) {}
    template <typename E>
    T operator () (E& e) const {
        T result = init;
        for (auto i : e) {
            result = f(result, i);
        }
        return result;
    }
};

template <typename F, typename T>
AggregateInit<F, T> iaggrerate(F f, T init) {
    return AggregateInit<F, T>(f, init);
}

// ===========================================================================

struct Max {
    template <typename U>
    U operator () (const U& u, const U& v) const {
        return v > u ? v : u;
    }
};

auto imax = iaggrerate(Max());

// ===========================================================================

struct Min {
    template <typename U>
    U operator () (const U& u, const U& v) const {
        return v < u ? v : u;
    }
};

auto imin = iaggrerate(Min());

// ===========================================================================

struct Sum {
    template <typename U, typename V>
    U operator () (const U& u, const V& v) const {
        return u + v;
    }
};

template <typename T>
auto isum(const T& init) {
    return iaggrerate(Sum(), init);
}

auto isum() {
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
auto icount(const T& x, const S& init = 0) {
    return iaggrerate(Count<T>(x), init);
}

// ===========================================================================

template <typename T, typename R>
struct Concat {
    T split;
    Concat(const T& s): split(s) {}
    template <typename E>
    R operator () (E& e) const {
        R result;
        if (!e) {
            return result;
        }
        result += e.current();
        e.advance();
        for (auto i : e) {
            result += split;
            result += i;
        }
        return result;
    }
};

template <typename R, typename T>
auto iconcat(const T& t) {
    return Concat<T, R>(t);
}
auto iconcat(char c, int n = 1) {
    return iconcat<std::string>(std::string(n, c));
}
auto iconcat(const char* s) {
    return iconcat<std::string>(std::string(s));
}
auto iconcat() {
    return iconcat("");
}

// ===========================================================================

template <typename F>
struct All {
    F f;
    All(const F& f): f(f) {}
    template <typename E>
    bool operator () (E& e) const {
        for (auto i : e) {
            if (!i) {
                return false;
            }
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
        for (auto i : e) {
            if (i) {
                return true;
            }
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
    MemberHolder(M m): m(m) {}
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
    , StdEnum<typename std::vector<typename E::ValueT>::reverse_iterator> {
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
    , StdEnum<typename std::vector<typename E::ValueT>::iterator> {
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
auto isortby(F cmp) {
    return SortBy<F>(cmp);
}

// ===========================================================================

// ===========================================================================

NS_ZL_END

#endif // ZPIPE_H
