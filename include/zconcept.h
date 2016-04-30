#ifndef ZCONCEPT_H
#define ZCONCEPT_H

#include <type_traits>
#include <utility>
#include <iosfwd>
#include <boost/type_traits.hpp>

#include "zcommon.h"

NS_ZL_BEGIN

#define CONCEPT_CHECK(C) std::enable_if_t<C>

template <typename T>
const bool CanCout = boost::has_left_shift<std::ostream, T>::value;

NS_ZL_END

#endif // ZCONCEPT_H
