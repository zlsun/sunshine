#ifndef ZDEBUG_H
#define ZDEBUG_H

#include <iostream>
#include <boost/type_index.hpp>

#include "zlog.h"

template <typename T>
std::string typestr()
{
    using boost::typeindex::type_id_with_cvr;
    return type_id_with_cvr<T>().pretty_name();
}

#define TYPE(T) (typestr<T>())
#define TYPEOF(o) (typestr<decltype(o)>())

#define DEBUG(x) zlogf("[%1%:%2%][%3%] %4%") __FILE__, __LINE__, __FUNCTION__, #x; x
#define DEBUG_VAR(x) zlogf("%1% = %2%") #x, x;
#define DEBUG_FUNC zlogf("%1%") __PRETTY_FUNCTION__;

#endif // ZDEBUG_H
