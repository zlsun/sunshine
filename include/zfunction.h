#ifndef ZFUNCTION_H
#define ZFUNCTION_H

#include <functional>
#include <type_traits>
#include "zcommon.h"

NS_ZL_BEGIN

namespace detail {
#if defined(_GLIBCXX_FUNCTIONAL)
#  define MEM_FN_SYMBOL std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
#  define MEM_FN_SYMBOL std::__mem_fn
#endif
#ifdef MEM_FN_SYMBOL
    template <typename F>
    using UniverseFunctionType = std::conditional_t<std::is_member_pointer<F>::value,
        MEM_FN_SYMBOL<F>,
        std::function<F>
    >;
#undef MEM_FN_SYMBOL
#endif
}

template <typename F>
struct Function: detail::UniverseFunctionType<F> {
    using BaseType = detail::UniverseFunctionType<F>;
    template <typename... Args>
    Function(Args... args): BaseType(args...) {}
};

NS_ZL_END

#endif // ZFUNCTION_H
