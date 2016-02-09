#ifndef ZTYPE_H
#define ZTYPE_H

#include <iostream>
#include <boost/type_index.hpp>

#include "zcommon.h"
#include "zstring.h"

NS_ZL_BEGIN

// ============================================================================

template <class... Types>
struct Typelist {};

// ============================================================================

template <class TList>
struct HeadImpl;

template <class H, class... T>
struct HeadImpl<Typelist<H, T...>> {
    using Type = H;
};

template <class TList>
using Head = typename HeadImpl<TList>::Type;

// ============================================================================

template <class TList>
struct TailImpl;

template <class H, class... T>
struct TailImpl<Typelist<H, T...>> {
    using Type = Typelist<T...>;
};

template <class TList>
using Tail = typename TailImpl<TList>::Type;

// ============================================================================

template <class TList>
struct LengthImpl;

template <class... Types>
struct LengthImpl<Typelist<Types...>> {
    enum { value = sizeof...(Types) };
};

template <class TList>
size_t Length = LengthImpl<TList>::value;

// ============================================================================

template <size_t idx, class TList>
struct TypeAtImpl;

template <size_t idx, class Head, class... Tail>
struct TypeAtImpl<idx, Typelist<Head, Tail...>> {
    using Type = typename TypeAtImpl<idx - 1, Typelist<Tail...>>::Type;
};

template <class Head, class... Tail>
struct TypeAtImpl<0, Typelist<Head, Tail...>> {
    using Type = Head;
};

template <size_t idx, class TList>
using TypeAt = typename TypeAtImpl<idx, TList>::Type;

// ============================================================================

template <class T, class TList>
struct IndexOfImpl;

template <class T>
struct IndexOfImpl<T, Typelist<>> {
    enum { value = -1 };
};

template <class T, class... Tail>
struct IndexOfImpl<T, Typelist<T, Tail...>> {
    enum { value = 0 };
};

template <class T, class Head, class... Tail>
struct IndexOfImpl<T, Typelist<Head, Tail...>> {
private:
    enum { temp = IndexOfImpl<T, Typelist<Tail...>>::value };
public:
    enum { value = temp == -1 ? -1 : 1 + temp };
};

template <class T, class TList>
int IndexOf = IndexOfImpl<T, TList>::value;

// ============================================================================

template <class T, class TList>
struct PrependImpl;

template <class T, class... Types>
struct PrependImpl<T, Typelist<Types...>> {
    using Type = Typelist<T, Types...>;
};

template <class T, class TList>
using Prepend = typename PrependImpl<T, TList>::Type;

// ============================================================================

template <class T, class TList>
struct AppendImpl;

template <class T, class... Types>
struct AppendImpl<T, Typelist<Types...>> {
    using Type = Typelist<Types..., T>;
};

template <class T, class TList>
using Append = typename AppendImpl<T, TList>::Type;

// ============================================================================

template <class TList>
struct PopImpl;

template <>
struct PopImpl<Typelist<>> {
    using Type = Typelist<>;
};

template <class T>
struct PopImpl<Typelist<T>> {
    using Type = Typelist<>;
};

template <class Head, class... Tail>
struct PopImpl<Typelist<Head, Tail...>> {
    using Type = Prepend<
        Head,
        typename PopImpl<Typelist<Tail...>>::Type
    >;
};

template <class TList>
using Pop = typename PopImpl<TList>::Type;

// ============================================================================

template <class TListA, class TListB>
struct ConcatImpl;

template <class... TypesA, class... TypesB>
struct ConcatImpl<Typelist<TypesA...>, Typelist<TypesB...>> {
    using Type = Typelist<TypesA..., TypesB...>;
};

template <class TListA, class TListB>
using Concat = typename ConcatImpl<TListA, TListB>::Type;

// ============================================================================

template <size_t idx, class T, class TList>
struct InsertImpl;

template <class T, class Head, class... Tail>
struct InsertImpl<0, T, Typelist<Head, Tail...>> {
    using Type = Typelist<T, Head, Tail...>;
};

template <size_t idx, class T, class Head, class... Tail>
struct InsertImpl<idx, T, Typelist<Head, Tail...>> {
    using Type = typename Prepend<
        Head,
        typename InsertImpl<
            idx - 1, T, Typelist<Tail...>
        >::Type
    >::Type;
};

template <size_t idx, class T, class TList>
using Insert = typename InsertImpl<idx, T, TList>::Type;

// ============================================================================

template <size_t idx, class TList>
struct DeleteImpl;

template <class Head, class... Tail>
struct DeleteImpl<0, Typelist<Head, Tail...>> {
    using Type = Typelist<Tail...>;
};

template <size_t idx, class Head, class... Tail>
struct DeleteImpl<idx, Typelist<Head, Tail...>> {
    using Type = typename Append<
        Head,
        typename DeleteImpl<
            idx - 1, Typelist<Tail...>
        >::Type
    >::Type;
};

template <size_t idx, class TList>
using Delete = typename DeleteImpl<idx, TList>::Type;

// ============================================================================

template <template <class> class Functor, class TList>
struct Map;

template <template <class> class Functor, class... Types>
struct Map<Functor, Typelist<Types...>> {
    using Type = Typelist<Functor<Types>...>;
};

// ============================================================================

template <template <class> class Functor, class TList>
struct ForEach;

template <template <class> class Functor, class... Types>
struct ForEach<Functor, Typelist<Types...>> {
    template <class F>
    ForEach(F f) {
        apply<Types...>(f);
    }
private:
    template <class F>
    void apply(F f) {
    }
    template <class H, class... T, class F>
    void apply(F f) {
        f(Functor<H>()());
        apply<T...>(f);
    }
};

// ============================================================================

template <class T>
struct TypeToStr {
    String operator () () {
        return boost::typeindex::type_id_with_cvr<T>().pretty_name();
    }
};

template <class TList>
String ToStringImpl() {
    String str = "<";
    bool first = true;
    ForEach<TypeToStr, TList>([&](const String& s) {
        if (!first) str += ", ";
        first = false;
        str += s;
    });
    str += ">";
    return str;
}

template <class TList>
String ToString = ToStringImpl<TList>();

NS_ZL_END

#endif // ZTYPE_H
