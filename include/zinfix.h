#ifndef ZINFIX_H
#define ZINFIX_H

#include <functional>

#include "zcommon.h"
#include "ztraits.h"
#include "zinit.h"

NS_ZL_BEGIN

template <class F>
struct Infix {

    using LT = typename function_traits<F>::template arg<0>::type;
    using RT = typename function_traits<F>::template arg<1>::type;
    using R = typename function_traits<F>::result_type;

    using FC = std::function<F>;

    template <class BF>
    struct BoundInfix {
        BF func;
        BoundInfix(BF func): func(std::move(func)) {}
    };

    FC func;

    Infix(FC f) :func(std::move(f)) {}

    auto bindLeft(LT lhs) const
    {
        auto b = std::bind(func, lhs, std::placeholders::_1);
        return BoundInfix<decltype(b)>(std::move(b));
    }

    auto bindRight(RT rhs) const
    {
        auto b = std::bind(func, std::placeholders::_1, rhs);
        return BoundInfix<decltype(b)>(std::move(b));
    }

    using BL = typename function_traits<decltype(&Infix::bindLeft)>::result_type;
    using BR = typename function_traits<decltype(&Infix::bindRight)>::result_type;

#define DEF_INFIX_OPERATOR(op)                           \
    friend auto operator op (LT lhs, const Infix& infix) \
    {                                                    \
        return infix.bindLeft(lhs);                      \
    }                                                    \
    friend auto operator op (const Infix& infix, RT rhs) \
    {                                                    \
        return infix.bindRight(rhs);                     \
    }                                                    \
    friend auto operator op (const BL& bl, RT rhs)       \
    {                                                    \
        return bl.func(rhs);                             \
    }                                                    \
    friend auto operator op (LT lhs, const BR& br)       \
    {                                                    \
        return br.func(lhs);                             \
    }

    DEF_INFIX_OPERATOR(+)
    DEF_INFIX_OPERATOR(-)
    DEF_INFIX_OPERATOR(*)
    DEF_INFIX_OPERATOR(/)
    DEF_INFIX_OPERATOR(%)

    DEF_INFIX_OPERATOR(&)
    DEF_INFIX_OPERATOR(|)
    DEF_INFIX_OPERATOR(^)

    DEF_INFIX_OPERATOR(<)
    DEF_INFIX_OPERATOR(>)
    DEF_INFIX_OPERATOR(<=)
    DEF_INFIX_OPERATOR(>=)
    DEF_INFIX_OPERATOR(==)

    DEF_INFIX_OPERATOR(<<)
    DEF_INFIX_OPERATOR(>>)

    DEF_INFIX_OPERATOR(&&)
    DEF_INFIX_OPERATOR(||)

#undef DEF_INFIX_OPERATOR
};


NS_ZL_END

#define INFIX(name, F)                            \
    static typename zl::Infix<F>::R               \
    _infix_##name(typename zl::Infix<F>::LT lhs,  \
                  typename zl::Infix<F>::RT rhs); \
    static zl::Infix<F> name(_infix_##name);      \
    typename zl::Infix<F>::R                      \
    _infix_##name(typename zl::Infix<F>::LT lhs,  \
                  typename zl::Infix<F>::RT rhs)

#endif // ZINFIX_H
