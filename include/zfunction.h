#ifndef ZFUNCTION_H
#define ZFUNCTION_H

#include <functional>
#include <type_traits>

#include "zcommon.h"

NS_ZL_BEGIN

namespace detail
{
#if defined(_GLIBCXX_FUNCTIONAL)
#  define MEM_FN_SYMBOL std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
#  define MEM_FN_SYMBOL std::__mem_fn
#endif
#ifdef MEM_FN_SYMBOL
    template <typename F>
    using UniverseFunctionType =
        std::conditional_t<
            std::is_member_pointer<F>::value,
            MEM_FN_SYMBOL<F>,
            std::function<F>
        >;
#else
    template <typename F>
    using UniverseFunctionType = std::function<F>;
#endif
#undef MEM_FN_SYMBOL
}

template <typename F>
class Function : public detail::UniverseFunctionType<F>
{
public:
    using BaseType = detail::UniverseFunctionType<F>;
    template <typename... Args>
    Function(Args&&... args)
        : BaseType(std::forward<Args>(args)...) {}
};

NS_ZL_END

#endif // ZFUNCTION_H
