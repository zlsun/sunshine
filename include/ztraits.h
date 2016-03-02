// cloned from https://github.com/kennytm/utils/blob/master/traits.hpp

#ifndef ZTRAITS_H
#define ZTRAITS_H

#include <cstdlib>
#include <tuple>
#include <functional>
#include <type_traits>

#include "zcommon.h"

NS_ZL_BEGIN

namespace detail
{
    template <typename C, typename R, typename... A>
    struct memfn_type
    {
        typedef typename std::conditional<
            std::is_const<C>::value,
            typename std::conditional<
                std::is_volatile<C>::value,
                R (C::*)(A...) const volatile,
                R (C::*)(A...) const
            >::type,
            typename std::conditional<
                std::is_volatile<C>::value,
                R (C::*)(A...) volatile,
                R (C::*)(A...)
            >::type
        >::type type;
    };
}

template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};

template <typename ReturnType, typename... Args>
struct function_traits<ReturnType(Args...)>
{
    typedef ReturnType result_type;

    typedef ReturnType function_type(Args...);

    template <typename OwnerType>
    using member_function_type = typename detail::memfn_type<
        typename std::remove_pointer<typename std::remove_reference<OwnerType>::type>::type,
        ReturnType, Args...
    >::type;

    enum { arity = sizeof...(Args) };

    template <size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

template <typename ReturnType, typename... Args>
struct function_traits<ReturnType(*)(Args...)>
    : public function_traits<ReturnType(Args...)>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...)>
    : public function_traits<ReturnType(Args...)>
{
    typedef ClassType& owner_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
    : public function_traits<ReturnType(Args...)>
{
    typedef const ClassType& owner_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) volatile>
    : public function_traits<ReturnType(Args...)>
{
    typedef volatile ClassType& owner_type;
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const volatile>
    : public function_traits<ReturnType(Args...)>
{
    typedef const volatile ClassType& owner_type;
};

template <typename FunctionType>
struct function_traits<std::function<FunctionType>>
    : public function_traits<FunctionType>
{};

#if defined(_GLIBCXX_FUNCTIONAL)
# define MEM_FN_SYMBOL std::_Mem_fn
#elif defined(_LIBCPP_FUNCTIONAL)
# define MEM_FN_SYMBOL std::__mem_fn
#endif

#ifdef MEM_FN_SYMBOL

template <typename R, typename C>
struct function_traits<MEM_FN_SYMBOL<R C::*>>
    : public function_traits<R(C*)>
{};
template <typename R, typename C, typename... A>
struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...)>>
    : public function_traits<R(C*, A...)>
{};
template <typename R, typename C, typename... A>
struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...) const>>
    : public function_traits<R(const C*, A...)>
{};
template <typename R, typename C, typename... A>
struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...) volatile>>
    : public function_traits<R(volatile C*, A...)>
{};
template <typename R, typename C, typename... A>
struct function_traits<MEM_FN_SYMBOL<R(C::*)(A...) const volatile>>
    : public function_traits<R(const volatile C*, A...)>
{};

#undef MEM_FN_SYMBOL
#endif

template <typename T>
struct function_traits<T&> : public function_traits<T> {};
template <typename T>
struct function_traits<const T&> : public function_traits<T> {};
template <typename T>
struct function_traits<volatile T&> : public function_traits<T> {};
template <typename T>
struct function_traits<const volatile T&> : public function_traits<T> {};
template <typename T>
struct function_traits<T&&> : public function_traits<T> {};
template <typename T>
struct function_traits<const T&&> : public function_traits<T> {};
template <typename T>
struct function_traits<volatile T&&> : public function_traits<T> {};
template <typename T>
struct function_traits<const volatile T&&> : public function_traits<T> {};

#define FORWARD_RES                               \
    typename std::conditional<                    \
        std::is_lvalue_reference<R>::value,       \
        T&,                                       \
        typename std::remove_reference<T>::type&& \
    >::type

template <typename R, typename T>
FORWARD_RES forward_like(T&& input) noexcept
{
    return static_cast<FORWARD_RES>(input);
}

#undef FORWARD_RES

template <typename From, typename To>
struct copy_cv
{
private:
    typedef typename std::remove_cv<To>::type raw_To;
    typedef typename std::conditional<std::is_const<From>::value,
                                      const raw_To, raw_To>::type const_raw_To;
public:
    typedef typename std::conditional<std::is_volatile<From>::value,
                                      volatile const_raw_To, const_raw_To>::type type;
};

template <typename T>
struct pointee
{
    typedef typename std::remove_reference<decltype(*std::declval<T>())>::type type;
};

template <typename T>
typename std::add_rvalue_reference<T>::type rt_val() noexcept
{
    return std::move(*static_cast<T*>(nullptr));
}

template <typename Op>
struct is_unary_function
{
    enum { value = function_traits<Op>::arity == 1 };
};

template <typename Op>
struct is_binary_function
{
    enum { value = function_traits<Op>::arity == 2 };
};

NS_ZL_END

#endif // ZTRAITS_H
