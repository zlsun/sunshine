#ifndef ZINFIX_H
#define ZINFIX_H

#include <functional>

#include "zcommon.h"
#include "ztraits.h"
#include "zinit.h"
#include "zlog.h"

NS_ZL_BEGIN

template <class F>
struct Infix
{
public:
    using LT = typename function_traits<F>::template arg<0>::type;
    using RT = typename function_traits<F>::template arg<1>::type;
    using R = typename function_traits<F>::result_type;
protected:
    LT left;
    F* func;
public:
    Infix<F>& addLeft(LT& lhs)
    {
        left = lhs;
        return *this;
    }
    Infix<F>& setFunc(F f) {
        func = f;
        return *this;
    }
    friend Infix<F>& operator | (LT lhs, Infix<F>& infix)
    {
        return infix.addLeft(lhs);
    }
    friend R operator | (Infix<F>& infix, RT rhs)
    {
        return infix.func(infix.left, rhs);
    }
};

NS_ZL_END


#define INFIX(name, F)                          \
    zl::Infix<F> name;                          \
    typename zl::Infix<F>::R                    \
    _infix##name(typename zl::Infix<F>::LT lhs, \
                typename zl::Infix<F>::RT rhs); \
    INIT {                                      \
        name.setFunc(_infix##name);             \
    }                                           \
    typename zl::Infix<F>::R                    \
    _infix##name(typename zl::Infix<F>::LT lhs, \
                typename zl::Infix<F>::RT rhs)

#endif // ZINFIX_H
