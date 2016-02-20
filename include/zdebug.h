#ifndef ZDEBUG_H
#define ZDEBUG_H

#include <iostream>

#include "ztype.h"
#include "zlog.h"
#include "zstring.h"

#define TYPE(T) (zl::TypeToStr<T>()())
#define TYPEOF(o) (TYPE(decltype(o)))

#ifdef NDEBUG
# define DEBUG(x)     x
# define DEBUG_VAR(x)
# define DEBUG_FUNC
#else
# define DEBUG(x)     zlog "[%1%:%2%][%3%] %4%"_f % __FILE__ % __LINE__ % __FUNCTION__ % #x; x
# define DEBUG_VAR(x) zlog "%1% = %2%"_f % #x % x;
# define DEBUG_FUNC   zlog "%1%"_f % __PRETTY_FUNCTION__;
#endif

#endif // ZDEBUG_H
