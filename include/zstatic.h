#ifndef ZSTATIC_H
#define ZSTATIC_H

#include <functional>

#include "../include/zcommon.h"

NS_ZL_BEGIN

namespace detail
{

    template <class R>
    struct StaticBlock
    {
        std::function<R()> block;
        template <class F>
        StaticBlock(F&& f): block(std::forward<F>(f)) {}
        decltype(auto) operator () () const
        {
            return block();
        }
    };

    template <bool>
    struct StaticBranch
    {
        template <class F>
        decltype(auto) operator * (F&& f)
        {
            return *this;
        }
    };
    template <>
    struct StaticBranch<true>
    {
        template <class F>
        decltype(auto) operator * (F&& f)
        {
            return StaticBlock<decltype(f())>(std::forward<F>(f));
        }
    };

    template <class T>
    struct StaticResult
    {
        T result;
        StaticResult(T&& t): result(std::move(t)) {}
        operator T ()
        {
            return std::move(result);
        }
        template <class U>
        decltype(auto) operator + (U&&)
        {
            return *this;
        }
    };
    template <>
    struct StaticResult<void>
    {
        template <class U>
        decltype(auto) operator + (U&&)
        {
            return *this;
        }
    };

    struct StaticExecutor
    {
        template <class R>
        decltype(auto) operator + (const StaticBlock<R>& block)
        {
            return StaticResult<R>(block());
        }
        decltype(auto) operator + (const StaticBlock<void>& block)
        {
            block();
            return StaticResult<void>();
        }
        decltype(auto) operator + (const StaticBranch<false>&)
        {
            return *this;
        }
    };

}

NS_ZL_END

#define STATIC_IF(pred)   zl::detail::StaticExecutor() + zl::detail::StaticBranch<pred>() * [&]()
#define STATIC_ELIF(pred) + zl::detail::StaticBranch<pred>() * [&]()
#define STATIC_ELSE       STATIC_ELIF(true)

#endif // ZSTATIC_H
