#ifndef ZLAZY_H
#define ZLAZY_H

#include <utility>
#include <exception>

#include "zcommon.h"

NS_ZL_BEGIN

class LazyNotInitException: public std::exception
{
public:
    virtual const char* what() const noexcept
    {
        return "Lazy not init";
    }
};

template <typename T>
class LazyInit
{
private:
    T* lazy = nullptr;
public:
    LazyInit() = default;
    LazyInit(const LazyInit&) = delete;
    ~LazyInit()
    {
        delete lazy;
    }
    LazyInit& operator = (const LazyInit&) = delete;
    LazyInit& operator = (T o)
    {
        delete lazy;
        lazy = new T(std::move(o));
        return *this;
    }
    operator T ()
    {
        if (!lazy) {
            throw LazyNotInitException();
        }
        T result = T(std::move(*lazy));
        delete lazy;
        lazy = nullptr;
        return result;
    }
};

NS_ZL_END

#endif // ZLAZY_H
