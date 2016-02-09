#ifndef ZINFIX_H
#define ZINFIX_H

#include <functional>

#include "zcommon.h"
#include "ztraits.h"
#include "zinit.h"

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
    Infix<F>& addLeft(LT lhs)
    {
        left = lhs;
        return *this;
    }

    Infix<F>& setFunc(F f)
    {
        func = f;
        return *this;
    }

#define DEF_INFIX_OPERATOR(op)                             \
    friend Infix<F>& operator op (LT lhs, Infix<F>& infix) \
    {                                                      \
        return infix.addLeft(lhs);                         \
    }                                                      \
    friend R operator op (Infix<F>& infix, RT rhs)         \
    {                                                      \
        return infix.func(infix.left, rhs);                \
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

    friend Infix<F>& operator , (LT lhs, Infix<F>& infix)
    {
        return infix.addLeft(lhs);
    }
    friend R operator , (Infix<F>& infix, RT rhs)
    {
        return infix.func(infix.left, rhs);
    }

#undef DEF_INFIX_OPERATOR

};

NS_ZL_END

#define INFIX(name, F)                            \
    static zl::Infix<F> name;                     \
    static typename zl::Infix<F>::R               \
    _infix_##name(typename zl::Infix<F>::LT lhs,  \
                  typename zl::Infix<F>::RT rhs); \
    INIT                                          \
    {                                             \
        name.setFunc(_infix_##name);              \
    }                                             \
    typename zl::Infix<F>::R                      \
    _infix_##name(typename zl::Infix<F>::LT lhs,  \
                  typename zl::Infix<F>::RT rhs)

#endif // ZINFIX_H
