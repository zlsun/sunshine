#ifndef ZTYPE_H
#define ZTYPE_H

#include <iostream>
#include <tuple>
#include <boost/type_index.hpp>

#include "zcommon.h"
#include "zstring.h"

NS_ZL_BEGIN

// ============================================================================

template <class... Types>
using TypeList = std::tuple<Types...>;

// ============================================================================

template <class TList>
struct HeadImpl;

template <class H, class... T>
struct HeadImpl<TypeList<H, T...>> {
    using Type = H;
};

template <class TList>
using Head = typename HeadImpl<TList>::Type;

// ============================================================================

template <class TList>
struct TailImpl;

template <class H, class... T>
struct TailImpl<TypeList<H, T...>> {
    using Type = TypeList<T...>;
};

template <class TList>
using Tail = typename TailImpl<TList>::Type;

// ============================================================================

template <class TList>
struct LengthImpl;

template <class... Types>
struct LengthImpl<TypeList<Types...>> {
    enum { value = sizeof...(Types) };
};

template <class TList>
size_t Length = LengthImpl<TList>::value;

// ============================================================================

template <size_t idx, class TList>
struct TypeAtImpl;

template <size_t idx, class Head, class... Tail>
struct TypeAtImpl<idx, TypeList<Head, Tail...>> {
    using Type = typename TypeAtImpl<idx - 1, TypeList<Tail...>>::Type;
};

template <class Head, class... Tail>
struct TypeAtImpl<0, TypeList<Head, Tail...>> {
    using Type = Head;
};

template <size_t idx, class TList>
using TypeAt = typename TypeAtImpl<idx, TList>::Type;

// ============================================================================

template <class T, class TList>
struct IndexOfImpl;

template <class T>
struct IndexOfImpl<T, TypeList<>> {
    enum { value = -1 };
};

template <class T, class... Tail>
struct IndexOfImpl<T, TypeList<T, Tail...>> {
    enum { value = 0 };
};

template <class T, class Head, class... Tail>
struct IndexOfImpl<T, TypeList<Head, Tail...>> {
private:
    enum { temp = IndexOfImpl<T, TypeList<Tail...>>::value };
public:
    enum { value = temp == -1 ? -1 : 1 + temp };
};

template <class T, class TList>
int IndexOf = IndexOfImpl<T, TList>::value;

// ============================================================================

template <class T, class TList>
struct PrependImpl;

template <class T, class... Types>
struct PrependImpl<T, TypeList<Types...>> {
    using Type = TypeList<T, Types...>;
};

template <class T, class TList>
using Prepend = typename PrependImpl<T, TList>::Type;

// ============================================================================

template <class T, class TList>
struct AppendImpl;

template <class T, class... Types>
struct AppendImpl<T, TypeList<Types...>> {
    using Type = TypeList<Types..., T>;
};

template <class T, class TList>
using Append = typename AppendImpl<T, TList>::Type;

// ============================================================================

template <class TList>
struct PopImpl;

template <>
struct PopImpl<TypeList<>> {
    using Type = TypeList<>;
};

template <class T>
struct PopImpl<TypeList<T>> {
    using Type = TypeList<>;
};

template <class Head, class... Tail>
struct PopImpl<TypeList<Head, Tail...>> {
    using Type = Prepend<
        Head,
        typename PopImpl<TypeList<Tail...>>::Type
    >;
};

template <class TList>
using Pop = typename PopImpl<TList>::Type;

// ============================================================================

template <class TListA, class TListB>
struct ConcatImpl;

template <class... TypesA, class... TypesB>
struct ConcatImpl<TypeList<TypesA...>, TypeList<TypesB...>> {
    using Type = TypeList<TypesA..., TypesB...>;
};

template <class TListA, class TListB>
using Concat = typename ConcatImpl<TListA, TListB>::Type;

// ============================================================================

template <size_t idx, class T, class TList>
struct InsertImpl;

template <class T, class Head, class... Tail>
struct InsertImpl<0, T, TypeList<Head, Tail...>> {
    using Type = TypeList<T, Head, Tail...>;
};

template <size_t idx, class T, class Head, class... Tail>
struct InsertImpl<idx, T, TypeList<Head, Tail...>> {
    using Type = typename Prepend<
        Head,
        typename InsertImpl<
            idx - 1, T, TypeList<Tail...>
        >::Type
    >::Type;
};

template <size_t idx, class T, class TList>
using Insert = typename InsertImpl<idx, T, TList>::Type;

// ============================================================================

template <size_t idx, class TList>
struct DeleteImpl;

template <class Head, class... Tail>
struct DeleteImpl<0, TypeList<Head, Tail...>> {
    using Type = TypeList<Tail...>;
};

template <size_t idx, class Head, class... Tail>
struct DeleteImpl<idx, TypeList<Head, Tail...>> {
    using Type = typename Append<
        Head,
        typename DeleteImpl<
            idx - 1, TypeList<Tail...>
        >::Type
    >::Type;
};

template <size_t idx, class TList>
using Delete = typename DeleteImpl<idx, TList>::Type;

// ============================================================================

template <template <class> class Functor, class TList>
struct Map;

template <template <class> class Functor, class... Types>
struct Map<Functor, TypeList<Types...>> {
    using Type = TypeList<Functor<Types>...>;
};

// ============================================================================

template <template <class> class Functor, class TList>
struct ForEach;

template <template <class> class Functor, class... Types>
struct ForEach<Functor, TypeList<Types...>> {
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
