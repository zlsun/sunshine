#ifndef ZCONCEPT_H
#define ZCONCEPT_H

#include <type_traits>
#include <utility>
#include <iosfwd>
#include <boost/type_traits.hpp>

#include "zcommon.h"

NS_ZL_BEGIN

namespace detail
{
    typedef char yes;
    typedef char (&no)[2];
    template <class T> yes check(T&&);
    no check(...);
}

#define CONCEPT_DEFINE(name, expr) \
    template <typename T>          \
    constexpr bool name = sizeof(detail::check(expr)) == sizeof(detail::yes);

#define CONCEPT_CHECK(C) std::enable_if_t<C>

template <typename T>
constexpr bool CanCout = boost::has_left_shift<std::ostream, T>::value;

NS_ZL_END

#endif // ZCONCEPT_H
